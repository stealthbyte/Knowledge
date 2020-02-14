import re

search_me = 'searchme - abcdefg'
my_re = re.compile('abcd')
match_obj = my_re.search(search_me)
if match_obj != None:
    print(search_me[match_obj.start() : match_obj.end()])
    
match_obj2 = re.search('abcd', 'searchme - abcdefg')
if match_obj2 != None:
    print(match_obj2.group(0))