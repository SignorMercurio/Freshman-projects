@echo off
WA.exe < Input.txt > WA.out
AC.exe < Input.txt > AC.out
fc WA.out AC.out
pause