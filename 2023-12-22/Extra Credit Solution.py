def try_val(mem, val, tgt):
    print(' Trying', val)
    if val == tgt:
        return True
    if mem[val]:
        return False
    nxt = 0
    if val % 2 == 0:
        nxt = val // 2
    else:
        nxt = val*3 + 1
    ret = try_val(mem, nxt, tgt)
    mem[val] = True
    return ret

def solve(tgt):
    mem = [False]*(10*tgt)
    mem[1] = True
    for i in range(2, tgt+1):
        print('Trying ', i)
        if try_val(mem, i, tgt):
            return i
    return 0 # Should never happen

print(solve(2024))
