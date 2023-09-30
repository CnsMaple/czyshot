# usage

1. 为了更好的体验，需要在hyprland中设置：

```conf
windowrule = pin,title:^(czyshot show)$
windowrule = float,^(czyshot)$
```

2. 需要安装wayshot和libnotify

```bash
paru -S wayshot
paru -S libnotify
```

3. 启动方式

`wayshot --stdout | czyshot`

4. 所有快捷键
 
任意时候：

`Esc`：退出任意窗口

`鼠标左键双击`：退出任意窗口

`Ctrl + c`：复制区域/贴图到剪贴板

`Ctrl + s`：保存区域/贴图到文件中

贴图前：

`鼠标左键`：选择区域

`鼠标中间`：将选择的区域贴图

`鼠标右键`：退出窗口

`Enter`：将选择的区域贴图

`Ctrl + a`：全选区域

贴图后：

