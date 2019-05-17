.source code.txt
.class public test
.super java/lang/Object

.method public <init>()V
aload_0
invokenonvirtual java/lang/Object/<init>()V
return
.end method

.method public static main([Ljava/lang/String;)V
.limit locals 100
.limit stack 100
.line 1
iconst_0
istore 1
.line 2
L_0:
ldc 5
istore 1
.line 3
L_1:
iload 1
ldc 5
if_icmpeq L_2
goto L_9
.line 4
L_2:
.line 5
ldc 100
istore 1
.line 6
L_3:
iconst_0
istore 2
.line 7
L_4:
ldc 10
istore 2
.line 8
L_5:
L_6:
iload 2
ldc 0
if_icmpge L_7
goto L_10
L_7:
.line 9
iload 1
ldc 1
iadd
istore 1
.line 10
L_8:
iload 2
ldc 1
isub
istore 2
.line 11
goto L_6
.line 12
goto L_10
.line 13
.line 14
L_9:
.line 15
ldc 9
istore 1
.line 16
.line 17
L_10:
return
.end method
