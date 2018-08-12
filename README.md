# Beslyric-for-X
Beslyric-for-X ，是 Beslyric 的跨平台升级版，打算基于QT实现。Beslyric 一款 操作简单、功能实用的 专门用于制作网易云音乐滚动歌词的 歌词制作软件。地址：https://github.com/BensonLaur/BesLyric

## 简单展示 

windows:

<img src="https://github.com/BensonLaur/image-storage/raw/master/beslyric-for-x/beslyric-for-x-demo-1.png" 
            width="70%" height="70%">
            
<img src="https://github.com/BensonLaur/image-storage/raw/master/beslyric-for-x/beslyric-for-x-demo-2.png" 
            width="70%" height="70%">

Linux(Debian/Ubuntu):

<img src="https://github.com/BensonLaur/image-storage/raw/master/beslyric-for-x/beslyric-for-x-demo-3.png" 
            width="70%" height="70%">
            
MacOs:

<img src="https://github.com/BensonLaur/image-storage/raw/master/beslyric-for-x/beslyric-for-x-demo-4.png" 
            width="70%" height="70%">

## 开发说明

1、clone 本项目

2、本项目使用跨平台开源库 ffmpeg 解析播放音频文件，考虑到更新ffmpeg的灵活性 以及 跨平台要求的特性，Beslyric-for-X 中在使用 ffmpeg 时，不直接将其置于项目下，而是开发者在对应的平台上各自独立单独安装。具体开发说明置于 [beslyic-lib](https://github.com/BensonLaur/beslyic-lib) 项目中。
