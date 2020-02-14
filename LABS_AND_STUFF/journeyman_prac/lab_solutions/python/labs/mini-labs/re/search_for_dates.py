import re

pattern = r"(19|20)\d\d[- /.](0[1-9]|1[012])[- /.](0[1-9]|[12][0-9]|3[01])"
my_re = re.compile(pattern)

data = open("History.txt").readlines()

num_matches_found = 0
for line in data:
    match_obj = my_re.search(line)
    if match_obj is None:
        continue
    num_matches_found += 1
    print(line, end='')

print()
print('%d matches found' % num_matches_found)
