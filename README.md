# 基于UE5.2 学习UE GAS开发

## 学习来源
 - [GameplayAbilitySystem_Aura](https://github.com/DruidMech/GameplayAbilitySystem_Aura)

## 相关说明
 - 并非完全照抄原教程的内容，加入了一些自己的想法和优化思路，简化了一些不必要的内容（加快学习进度）
 - 项目的目录结构，以及命名方式也存在不同
 - 暂时停止在最后的部分内容了，与GAS偏离很多了，没必要继续学后面的东西，很商业化开发偏离的也很多
 
## 其余文件
 - CenterConsole.ps1 将Windows 的控制台窗口生成在屏幕的中心，支持多显示屏，默认显示在主屏（用于后面的bat文件中）
 - GitPull_FromMaster.bat 将本地文件更新到最新的Master分支（顺便学习git已经多分支的使用）
 - GitPush_AddCommit.bat 将本地文件提交到远程仓库
 - GitPush_HasCommit.bat 已经执行 GitPush_AddCommit.bat，但可能失败：网络或者版本冲突，当修复好相关问题后，继续完成提交代码，则使用这个文件（国内存在访问github不太顺畅的情况，git add 和 git commit -m "" 操作在本地，一般没有问题，存在git push 有网络相关问题）
 - .gitignore 忽略一些不需要提交的文件（已经添加了个人认为不必要的文件）
