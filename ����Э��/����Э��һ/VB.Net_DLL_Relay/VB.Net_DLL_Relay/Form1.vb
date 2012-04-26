
Public Class Form1




    Private Declare Function DeviceQuaryCommand Lib "RelayControlCtl.dll" (ByVal quary_str As String, ByVal out As String, ByVal out_len As Integer) As Integer

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        RichTextBox1.Text = "set_relay_val ETH:TCP:192.168.1.250:2000 R1:ON R2:ON  R3:OFF"
    End Sub

    Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.Click
        Dim y As String
        '以下定义一个字符串，
        '为什么这么定义呢？其实就是为了使 y 变量占用一定的空间，也就是 VB6中常用的 Dim y As String*256
        '这样，DLL函数执行完毕，会返回一些数据，写回y变量中，256是y的长度，返回的字符串长度不会超过256字节,包括NULL结尾的一个字节
        y = "------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------"
        DeviceQuaryCommand(RichTextBox1.Text, y, 256)
        '返回数据，写回到对话框
        RichTextBox2.Text = y
    End Sub
End Class
