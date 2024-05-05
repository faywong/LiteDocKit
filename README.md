# LiteDocKit
一个支持双向同步的笔记管理软件（支持富文本格式 markdown、asciidoc 还有常见 c/c++、bash、bat、lua 等编程语言文件，同时具备向云端同步的能力）

它具备如下特性：

* 支持 [unison file synchronizer](https://github.com/bcpierce00/unison) 来进行与服务端的双向文档同步（Ctrl + Shift + S）
* 支持常见文件格式（markdown、asciidoc、c、c++、bash、bat、lua）的编辑和高亮、预览
* 支持 [pandoc](https://pandoc.org/) 支持的所有格式转换和导出（进行中）
* 支持主题色（solarized、gruvbox 进行中）
* [ripgrep](https://github.com/BurntSushi/ripgrep) 驱动的全局搜索（Ctrl + Shift + F）
* 支持 [org-capture 插件](https://chromewebstore.google.com/detail/kkkjlfejijcjgjllecmnejhogpbcigdc) 来捕获网页内容到本地文件夹

注意：本应用还在开发中，不可用于关键任务，请自甘风险。若造成相关损失，软件作者免于责任。

# build

## install dependency

```shell
sudo apt-get install cmake qt5-default qtcreator qtwebengine5-dev 
```

```shell
git clone --recurse-submodules git@github.com:faywong/syncfolder.git
mkdir build
cd build
cmake ..
```

# screenshot
![SerenityOS](https://i.imgur.com/T9kWTXB.png)

![](https://i.imgur.com/7OaXFws.png)
![](https://i.imgur.com/Cj7hzGR.png)
![](https://i.imgur.com/5KAwyVI.png)
![](https://i.imgur.com/AK1Ie5G.png)

# org-capture url example & setup
  
1. install org-capture extension on [chrome store](https://chrome.google.com/webstore/detail/kkkjlfejijcjgjllecmnejhogpbcigdc)

2. vim  ~/.local/share/applications/LiteDocKit.desktop

```shell

[Desktop Entry]
Name=lite-dockit
Comment=Your lite document kit, Intercept calls from emacsclient to trigger custom actions
Categories=Other;
Keywords=document management;org-protocol;
Icon=dockit
Type=Application
Exec=lite-dockit --orgcapture %u
Terminal=false
MimeType=x-scheme-handler/org-protocol;

```

faywong@gentoo ~ $ update-desktop-database ~/.local/share/applications/


