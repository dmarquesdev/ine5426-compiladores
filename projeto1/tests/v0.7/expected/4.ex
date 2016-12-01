
bool fun: myfunc (params: int y)
    bool var: resp
    if: < y 10
then: 
  = resp false

    ret resp

bool var: te
= te myfunc[1 params] 10

 