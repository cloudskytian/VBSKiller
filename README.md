# VBSKiller

[English](#english) | [中文](#中文)

---

## English

### Description
This is a simple UEFI application designed to disable **Virtualization-Based Security (VBS)** by setting the `VbsPolicyDisabled` UEFI variable before Windows boots. 

It is specifically intended for users who want to use **WSL2** or **Hyper-V** features but wish to keep VBS disabled.

After modifying the variable, the application automatically searches for the Windows Boot Manager (`BOOTMGFW.EFI`) and chain-loads it.

### Key Features
- **VBS Bypass**: Sets `VbsPolicyDisabled` to `1` in the Microsoft namespace.
- **Auto Boot**: Automatically detects the Windows partition and boots the OS.

### Usage Instructions
First of all, you need to disable Secure Boot in UEFI.

Download [VBSKiller.efi](https://github.com/cloudskytian/VBSKiller/releases/latest/download/VBSKiller.efi)

Use the following command to mount the ESP partition:

`mountvol X: /S`

Place `VBSKiller.efi` into `X:\EFI\Boot\`

Set `VBSKiller.efi` as the default boot entry using `efivarcli`, `BOOTICEx64`, `DiskGenius`, or any other method.

Using `efivarcli` as an example: after downloading [efivarcli-2.0-win.exe](https://github.com/iTrooz/efivar-rs/releases/download/v2.0/efivarcli-2.0-win.exe), execute the following command:

`efivarcli-2.0-win.exe boot add -f \EFI\Boot\VBSKiller.efi -d VBSKiller`

After rebooting, run `msinfo32.exe`. If VBS is shown as not enabled, the installation was successful.

### Build Instructions
To avoid potential compatibility issues or "unexpected behaviors" during compilation, it is highly recommended to use the **edk2-stable202208** tag of the EDK2 environment.

*Reference*: See [EfiGuard Issue #112](https://github.com/Mattiwatti/EfiGuard/issues/112) for details on why this version is preferred.

### Credits
This project references logic from [EfiGuard](https://github.com/Mattiwatti/EfiGuard).

Special thanks to [Mattiwatti](https://github.com/Mattiwatti) for their research into Windows boot security.

---

## 中文

### 项目简介
这是一个简单的 UEFI 应用程序，旨在通过在 Windows 启动前设置 `VbsPolicyDisabled` UEFI 变量来禁用 **VBS (基于虚拟化的安全性)**。

**适用场景**：本项目特别适用于那些需要启用 **WSL2** 或 **Hyper-V**，但希望在系统中禁用 VBS 的用户。

程序在修改变量后，会自动搜索 Windows 引导管理器 (`BOOTMGFW.EFI`) 并实现链式启动。

### 主要功能
- **VBS 绕过**：在微软 GUID 命名空间下将 `VbsPolicyDisabled` 设置为 `1`。
- **自动引导**：自动检测系统中存在的 Windows 分区并引导操作系统。

### 使用说明
首先，你需要在 UEFI 里禁用安全启动

下载 [VBSKiller.efi](https://github.com/cloudskytian/VBSKiller/releases/latest/download/VBSKiller.efi)

使用以下命令挂载 ESP 分区

`mountvol X: /S`

将 `VBSKiller.efi` 放置到 `X:\EFI\Boot\`

使用 `efivarcli`、 `BOOTICEx64`、 `DiskGenius` 或其他任何方式将 `VBSKiller.efi` 设置为默认启动项即可

这里以 `efivarcli` 为例，下载 [efivarcli-2.0-win.exe](https://github.com/iTrooz/efivar-rs/releases/download/v2.0/efivarcli-2.0-win.exe) 后执行以下命令即可：

`efivarcli-2.0-win.exe boot add -f \EFI\Boot\VBSKiller.efi -d VBSKiller`

重启后运行 `msinfo32.exe` 查看 VBS 为禁用状态即说明安装成功

### 编译说明
为了防止编译过程中出现意外问题或运行异常，建议使用 **edk2-stable202208** 标签的 EDK2 进行编译。

*参考建议*：参考 [EfiGuard Issue #112](https://github.com/Mattiwatti/EfiGuard/issues/112) 了解为何选择该特定版本的 EDK2。

### 致谢
本项目参考了 [EfiGuard](https://github.com/Mattiwatti/EfiGuard) 的实现逻辑。

特别感谢 [Mattiwatti](https://github.com/Mattiwatti) 在 Windows 启动安全领域的研究工作。
