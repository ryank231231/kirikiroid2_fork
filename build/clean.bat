@set PATH=C:\Program Files\Microsoft Visual Studio 10.0\VC;%PATH%
@set PATH=D:\Program Files\Microsoft Visual Studio 10.0\VC;%PATH%
@call vcvarsall.bat
@rmdir /S /Q ..\cocos\2d\Debug.win32
@rmdir /S /Q ..\external\zlib-1.2.5\Debug
@rmdir /S /Q ..\external\libpng-1.4.5\Debug
@rmdir /S /Q Debug.win32
@MSBuild cocos2d-win32.vc2010.sln /t:Clean /p:Configuration=Debug /p:Platform=win32
@pause

