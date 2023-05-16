def __unaryfunc_method(name: str):
    def func(x):
        try:
            f = type(x).__dict__[f"__{name}__"]
        except:
            raise AttributeError(
                f"Function __{name}__ is not implemented in {type(x)} object.")
        p = f(x)
        if p is NotImplemented:
            raise TypeError(
                f"Function {name} on {type(x)} object is not implemented")
        return f(x)
    return func


conj = __unaryfunc_method("conj")
exp = __unaryfunc_method("exp")
ln = __unaryfunc_method("ln")
sqrt = __unaryfunc_method("sqrt")
sin = __unaryfunc_method("sin")
cos = __unaryfunc_method("cos")
tan = __unaryfunc_method("tan")
cot = __unaryfunc_method("cot")
sec = __unaryfunc_method("sec")
csc = __unaryfunc_method("csc")
arcsin = __unaryfunc_method("arcsin")
arccos = __unaryfunc_method("arccos")
arctan = __unaryfunc_method("arctan")
arccot = __unaryfunc_method("arccot")
arcsec = __unaryfunc_method("arcsec")
arccsc = __unaryfunc_method("arccsc")
sinh = __unaryfunc_method("sinh")
cosh = __unaryfunc_method("cosh")
tanh = __unaryfunc_method("tanh")
coth = __unaryfunc_method("coth")
sech = __unaryfunc_method("sech")
csch = __unaryfunc_method("csch")
arcsinh = __unaryfunc_method("arcsinh")
arccosh = __unaryfunc_method("arccosh")
arctanh = __unaryfunc_method("arctanh")
arccoth = __unaryfunc_method("arccoth")
arcsech = __unaryfunc_method("arcsech")
arccsch = __unaryfunc_method("arccsch")


def __binaryfunc_method(name: str, has_r: bool = True):
    if has_r:
        def func(x, y):
            try:
                f = type(x).__dict__[f"__{name}__"]
            except:
                pass
            else:
                v = f(x, y)
                if v is NotImplemented:
                    raise TypeError(
                        f"Function {name} between {type(x)} object and {type(y)} object is not implemented")
                return v
            try:
                f = type(y).__dict__[f"__r{name}__"]
            except:
                raise AttributeError(
                    f"Both __{name}__ and __r{name}__ are not implemented in {type(x)} object.")
            else:
                v = f(y, x)
                if v is NotImplemented:
                    raise TypeError(
                        f"Function {name} between {type(x)} object and {type(y)} object is not implemented")
                return v
        return func
    else:
        def func(x, y):
            try:
                f = type(x).__dict__[f"__{name}__"]
            except:
                raise AttributeError(
                    f" __{name}__ is not implemented in {type(x)} object.")
            else:
                v = f(x, y)
                if v is NotImplemented:
                    raise TypeError(
                        f"Function {name} between {type(x)} object and {type(y)} object is not implemented")
                return v
        return func


log = __binaryfunc_method("log")
hadamard = __binaryfunc_method("hadamard", False)
kronecker = __binaryfunc_method("kronecker", False)
dot = __binaryfunc_method("dot", False)
