
def DeterminePlacementForRoundOfTwo():
    V = [[0 for j in range(21)] for i in range(2)]
    V[0][20] = 2 # The player that starts with 20 is eliminated and finishes 2nd
    V[1][20] = 1 # The other player is the winner
    for j in range(19, 0, -1): # iterate from 19 down to 1
        best_move = 0
        min_val = 3
        # find move which minimizes finishing rank
        for k in range(1, 5):
            if j + k > 20:
                break
            if V[1][j+k] < min_val:
                best_move = j+k
                min_val = V[1][j+k]
        V[0][j] = V[1][best_move]
        V[1][j] = V[0][best_move]
    # Starting player is required to pick 1
    V[0][0] = V[1][1]
    V[1][0] = V[0][1]
    return V

def DeterminePlacementForRoundOfN(N, V_prev):
    V = [[0 for j in range(21)] for i in range(N)]
    V[0][20] = N # The player that starts with 20 is eliminated and finishes Nth
    for p in range(1, N):
        V[p][20] = V_prev[p-1][0] # The other players move on to the next round, finishing in the order that has been determined in V_prev
    for j in range(19, 0, -1): # iterate from 19 down to 1
        best_move = 0
        min_val = N+1
        # find move which minimizes finishing rank
        for k in range(1, 5):
            if j + k > 20:
                break
            if V[N-1][j+k] < min_val:
                best_move = j+k
                min_val = V[N-1][j+k]
        for p in range(N):
            V[p][j] = V[(p - 1) % N][best_move]
    # Starting player is required to pick 1
    for p in range(N):
        V[p][0] = V[(p - 1) % N][1]
    return V

def PrintPlacement(N, V):
    print('---- Round of', N, '----')
    for p in range(0, N):
        print('Player', p+1, ':', V[p][0])
    print('')

def main():
    V = DeterminePlacementForRoundOfTwo()
    PrintPlacement(2, V)
    for num_players in range(3, 50):
        V = DeterminePlacementForRoundOfN(num_players, V)
        PrintPlacement(num_players, V)

if __name__ == "__main__":
    main()
