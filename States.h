

/*
  MIT Licence
Author -> Brett Rogers

*/

namespace Snake
{

  struct GameState
  {
     enum States
    {
        Title = 1,
        Game,
        End,
    };

    int currentGameState = 0;
  };
   

}