def append_to(element, to=[]):
    to.append(element)
    
    return to
    
print(append_to(12))
print(append_to(42))


def append_to_correct(element, to=None):
    if to is None:
        to = []
    to.append(element)
    
    return to