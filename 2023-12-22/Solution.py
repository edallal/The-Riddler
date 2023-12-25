import math

def Solve(tgt):
    max_vol = 0
    amax = math.floor(tgt/math.sqrt(3))
    for a in range(amax, 0, -1):
        bmax = math.floor(math.sqrt(tgt*tgt-a*a))
        for b in range(bmax, a-1, -1):
            c = round(math.sqrt(tgt*tgt-a*a-b*b))
            if c < b:
                continue
            if a*a + b*b + c*c == tgt*tgt:
                print('({}, {}, {})'.format(a, b, c))
                max_vol = max(max_vol, a*b*c)
                break # Any further solutions will have lower volume
    return max_vol

print(8**3*Solve(253))
