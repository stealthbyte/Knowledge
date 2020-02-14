import random

print(random.randint(0, 10))
nums = list(range(1, 6))
print(nums)
random.shuffle(nums)
print(nums)

for i in random.sample(nums, 3):
    print(i)