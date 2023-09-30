#include "util/util.h"

bool RunPipe(const std::string &cmd, QPixmap &pixmap)
{
    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe)
    {
        qDebug() << "Failed to run command."
                 << "\n";
        return false;
    }

    QByteArray byteArray;
    char buffer[1024];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), pipe)) > 0)
    {
        byteArray.append(buffer, bytesRead);
    }

    pclose(pipe);

    QImage image;
    image.loadFromData(byteArray);

    pixmap = QPixmap::fromImage(image);
    return true;
}

bool copyPixmap(QImage image)
{
    int pipefd[2];
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return false;
    }

    pid_t child_pid = fork();
    if (child_pid == -1)
    {
        perror("fork");
        ::close(pipefd[0]);
        ::close(pipefd[1]);
        return false;
    }

    if (child_pid == 0)
    {
        ::close(pipefd[1]);

        if (dup2(pipefd[0], STDIN_FILENO) == -1)
        {
            perror("dup2");
            exit(1);
        }
        ::close(pipefd[0]);

        execlp("wl-copy", "wl-copy", "-t", "image/png", NULL);

        perror("execlp");
        exit(1);
    }
    else
    {
        ::close(pipefd[0]);

        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        buffer.open(QBuffer::WriteOnly);
        if (image.save(&buffer, "PNG"))
        {
            ssize_t bytes_written = write(pipefd[1], byteArray.constData(), byteArray.size());
            if (bytes_written == -1)
            {
                perror("write");
                ::close(pipefd[1]);
                return false;
            }
            ::close(pipefd[1]);

            int status;
            waitpid(child_pid, &status, 0);

            if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
            {
                qDebug() << "Image copied to clipboard successfully."
                         << "\n";

                system("notify-send 'czyshot' 'Image copied to clipboard successfully.' -u normal");
                return true;
            }
            else
            {
                qDebug() << "Failed to copy image to clipboard."
                         << "\n";

                system("notify-send 'czyshot' 'Failed to copy image to clipboard.' -u normal");
                return false;
            }
        }
        else
        {
            qDebug() << "Failed to save image."
                     << "\n";
            system("notify-send 'czyshot' 'Failed to save image.' -u normal");
            ::close(pipefd[1]);
            return false;
        }
    }

    return false;
}

bool savePixmap(QPixmap pixmap)
{
    // 获取当前日期和时间
    QDateTime currentDateTime = QDateTime::currentDateTime();

    // 转换为字符串以进行显示或进一步处理
    QString currentDateTimeString = currentDateTime.toString("yyyy-MM-dd_HH:mm:ss");
    // 获取用户的家目录路径
    QString homePath = QDir::homePath() + "/";
    // 打开文件保存对话框，并限制文件类型为PNG
    QString filePath =
        QFileDialog::getSaveFileName(nullptr,
                                     "保存文件",
                                     homePath + "czyshot-" + currentDateTimeString + ".png",
                                     "PNG Files (*.png)");

    // 用户选择了保存文件的路径后，检查是否有效，然后保存Pixmap
    if (!filePath.isEmpty())
    {
        qDebug() << "save path:" << filePath;
        // 检查文件路径是否以.png结尾
        if (!filePath.endsWith(".png", Qt::CaseInsensitive))
        {
            // 如果不是，可以做出相应的处理
            pixmap.save(filePath + ".png");
        }
        else
        {
            // 文件路径以.png结尾，可以保存Pixmap
            pixmap.save(filePath);
        }
        system("notify-send 'czyshot' 'save successfully.' -u normal");
        return true;
    }
    else
    {
        system("notify-send 'czyshot' 'no select path.' -u normal");
        return false;
    }
}
