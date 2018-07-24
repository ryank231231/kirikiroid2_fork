@set PATH=C:\Program Files\Microsoft Visual Studio 10.0\VC;%PATH%
@set PATH=D:\Program Files\Microsoft Visual Studio 10.0\VC;%PATH%
@call vcvarsall.bat
@MSBuild cocos2d-win32.vc2010.sln /t:Clean /p:Configuration=Debug /p:Platform=win32
@pause

