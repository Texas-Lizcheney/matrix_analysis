import typing

T = typing.TypeVar("T")
U = typing.TypeVar("U")


def conj(x: T) -> T:
    ...


def exp(x: T) -> T:
    ...


def ln(x: T) -> T:
    ...


def sqrt(x: T) -> T:
    ...


def sin(x: T) -> T:
    ...


def cos(x: T) -> T:
    ...


def tan(x: T) -> T:
    ...


def cot(x: T) -> T:
    ...


def sec(x: T) -> T:
    ...


def csc(x: T) -> T:
    ...


def arcsin(x: T) -> T:
    ...


def arccos(x: T) -> T:
    ...


def arctan(x: T) -> T:
    ...


def arccot(x: T) -> T:
    ...


def arcsec(x: T) -> T:
    ...


def arccsc(x: T) -> T:
    ...


def sinh(x: T) -> T:
    ...


def cosh(x: T) -> T:
    ...


def tanh(x: T) -> T:
    ...


def coth(x: T) -> T:
    ...


def sech(x: T) -> T:
    ...


def csch(x: T) -> T:
    ...


def arcsinh(x: T) -> T:
    ...


def arccosh(x: T) -> T:
    ...


def arctanh(x: T) -> T:
    ...


def arccoth(x: T) -> T:
    ...


def arcsech(x: T) -> T:
    ...


def arccsch(x: T) -> T:
    ...


def log(x: T, y: U) -> T | U:
    ...