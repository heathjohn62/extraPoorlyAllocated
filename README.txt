During the past two weeks,
John:
-Researched a way to look quickly look through a search tree
 -Found BFS and (ID)DFS, went on to see how feasible each one was to develop
 -decided on latter, implemented cautiously.
-Looked into ways of pruning tree to improve speed, alpha-beta plus a couple
  heuristic ways

Alex:
-Researched and implemented how to evaluate a board as accurately as possible
-Fine tuned recursive algorithm to do endgames.


First, we had a very primitive minimax function in addition to a very simple
evaluation function, one that would just make the move that yielded the most
tiles for us. Next, we tried a BFS search that used a heuristic pruning memory
and utilized many different features, but unfortunately this took up too much
space when we wanted to get to a reasonable depth. Thus we decided to optimize
the minimax with alpha-beta pruning and had we more time, would have tried to
choose the moves that would prune the most (refutation table like thing). We
also made a step up on our evaluation function to have board weights. However,
we quickly realized that this was not going to be enough to beat the best bots,
so we attempted to improve that by getting our program to understand the notion
of stable spaces, or ones that cannot be flipped. We hope this will put us near
the top of the leaderboards for the competition.
