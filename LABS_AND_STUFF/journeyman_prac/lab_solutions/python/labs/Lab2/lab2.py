import string
nums = [index for index in range(26)]
print('nums = %s' % nums)
print()

lowercase_letters = string.ascii_lowercase
print('lowercase letters = %s' % lowercase_letters)
print()

lowercase_letters_dict = {}
for index, letter in enumerate(lowercase_letters):
    lowercase_letters_dict[letter] = nums[index]

print('lowercase letters dictionary = %s' % lowercase_letters_dict)
print()

with open('mypairs.txt', 'w') as fh:
    for key in sorted(lowercase_letters_dict.keys()):
        fh.write('key = %s, value = %d\n' % (key, lowercase_letters_dict[key]))
    fh.write('\n')
    for key in sorted(lowercase_letters_dict.keys(), reverse=True):
        fh.write('key = %s, value = %d\n' % (key, lowercase_letters_dict[key]))
    fh.write('\n')

data = open('mypairs.txt').read()
print('mypairs.txt')
print('-----------')
print(data)
