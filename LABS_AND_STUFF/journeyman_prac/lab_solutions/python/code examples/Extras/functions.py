def is_odd(num):
    return num % 2 == 0

def add_dict(dict_a, b):
    dict_a.update(dict(b))

    return dict_a


def partial_apply(func, *args, **kwargs):
    mykwargs = dict(kwargs)
    myargs = tuple(args)

    def inner_f(*_args, **_kwargs):
        mykwargs.update(_kwargs)

        return func(*(_args+myargs), **mykwargs)

    inner_f.func_name = '%s_partial' % func.func_name

    return inner_f

def my_sum(*vals, **kwargs):
    multiplier = kwargs.get('multiplier', 1)

    return sum(vals) * multiplier