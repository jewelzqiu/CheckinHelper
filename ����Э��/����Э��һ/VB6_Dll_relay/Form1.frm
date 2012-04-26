VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3975
   ClientLeft      =   120
   ClientTop       =   450
   ClientWidth     =   11580
   LinkTopic       =   "Form1"
   ScaleHeight     =   3975
   ScaleWidth      =   11580
   StartUpPosition =   3  '窗口缺省
   Begin VB.TextBox Text2 
      Height          =   735
      Left            =   240
      TabIndex        =   2
      Top             =   2160
      Width           =   11055
   End
   Begin VB.TextBox Text1 
      Height          =   1215
      Left            =   240
      TabIndex        =   1
      Text            =   "Text1"
      Top             =   480
      Width           =   11055
   End
   Begin VB.CommandButton Command1 
      Caption         =   "发送指令"
      Height          =   735
      Left            =   4680
      TabIndex        =   0
      Top             =   3000
      Width           =   2175
   End
   Begin VB.Label Label2 
      Caption         =   "返回字符串："
      Height          =   255
      Left            =   120
      TabIndex        =   4
      Top             =   1800
      Width           =   1335
   End
   Begin VB.Label Label1 
      Caption         =   "输入指令："
      Height          =   255
      Left            =   120
      TabIndex        =   3
      Top             =   120
      Width           =   1095
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False


Private Declare Function DeviceQuaryCommand Lib "RelayControlCtl.dll" (ByVal quary_str As String, ByVal out As String, ByVal out_len As Integer) As Integer



Private Sub Command1_Click()
        Dim y As String * 1024
        b = DeviceQuaryCommand(Text1.Text, y, 1024)
        Text2.Text = y
End Sub



Private Sub Form_Load()
  Text1.Text = "set_relay_val   ETH : TCP: 192.168.1.250 :2000          R1: ON    R2:OFF    R3:OFF"
End Sub

