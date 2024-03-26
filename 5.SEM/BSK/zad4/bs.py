def check(s):
    for i in range(1, len(s)-1):
        before = s[:i]
        after = s[i+1:]
        if min(before) < s[i] and min(after) < s[i]:
            return False
        if max(before) > s[i] and max(after) > s[i]:
            return False
    return True

ile = 0
for let1 in 'abcdefghijklmnopqrstuvwxyz':
    str = let1
    if not check(str):
        ile += 1
        # print(str)
print(ile)

for let1 in 'abcdefghijklmnopqrstuvwxyz':
    for let2 in 'abcdefghijklmnopqrstuvwxyz':
        str = let1 + let2
        if not check(str):
            ile += 1
            # print(str)
print(ile)

for let1 in 'abcdefghijklmnopqrstuvwxyz':
    for let2 in 'abcdefghijklmnopqrstuvwxyz':
        for let3 in 'abcdefghijklmnopqrstuvwxyz':
            str = let1 + let2 + let3
            if not check(str):
                ile += 1
                # print(str)
print(ile)

for let1 in 'abcdefghijklmnopqrstuvwxyz':
    for let2 in 'abcdefghijklmnopqrstuvwxyz':
        for let3 in 'abcdefghijklmnopqrstuvwxyz':
            for let4 in 'abcdefghijklmnopqrstuvwxyz':
                str = let1 + let2 + let3 + let4
                if not check(str):
                    ile += 1
                    # print(str)
print(ile)

for let1 in 'abcdefghijklmnopqrstuvwxyz':
    for let2 in 'abcdefghijklmnopqrstuvwxyz':
        for let3 in 'abcdefghijklmnopqrstuvwxyz':
            for let4 in 'abcdefghijklmnopqrstuvwxyz':
                for let5 in 'abcdefghijklmnopqrstuvwxyz':
                    str = let1 + let2 + let3 + let4 + let5
                    if not check(str):
                        ile += 1
                        # print(str)
print(ile)