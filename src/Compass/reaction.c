#include "reaction.h"
#include "sseg.h"
#include "gpio.h"
#include "utils.h"
#include "state.h"

int reactCountdown;

/// Play the easter egg game!
///
/// Plays the reaction game where the user must hit pushbutton 2 as close to
/// the zero mark as possible.  A message is displayed giving feedback on how
/// close the player got.
void playReactionGame()
{
  displayCountdown = 1;
  while(displayCountdown);

  writeMessage(SSEG_BE);
  displayCountdown = 2;
  while(displayCountdown);

  writeMessage(SSEG_FAST);
  displayCountdown = 2;
  while(displayCountdown);

  reactCountdown = 1000;
  while (reactCountdown)
  {
    writeInt(reactCountdown);
    if (readPushButton(PUSHBUTTON_2))
    {
      reactCountdown *= -1;
      break;
    }
  }
  if (reactCountdown == 0)
  {
    writeMessage(SSEG_BAD);
    displayCountdown = 2;
    while(displayCountdown);
  }
  else
  {
    int score = -reactCountdown;
    writeInt(score);
    displayCountdown = 2;
    while(displayCountdown);
    if (score < 5)
    {
      writeMessage(SSEG_GOOD);
      displayCountdown = 2;
      while(displayCountdown);
    }
    else if (score < 15)
    {
      writeMessage(SSEG_NICE);
      displayCountdown = 2;
      while(displayCountdown);
    }
    else if (score < 50)
    {
      writeMessage(SSEG_EH);
      displayCountdown = 2;
      while(displayCountdown);
    }
    else
    {
      writeMessage(SSEG_BAD);
      displayCountdown = 2;
      while(displayCountdown);
    }
  }
}
