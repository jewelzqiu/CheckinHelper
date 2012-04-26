Attribute VB_Name = "Module1"


Private aaa As Integer
Private bbb As Integer

Public Function get_aaa() As Integer
  aaa = aaa + 1
  get_aaa = aaa
End Function
