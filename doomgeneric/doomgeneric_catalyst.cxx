
#include "CatalystAdaptor.h"
#include "doomkeys.h"

#include <cstdint>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#define KEYQUEUE_SIZE 16

static unsigned short s_KeyQueue[KEYQUEUE_SIZE];
static unsigned int s_KeyQueueWriteIndex = 0;
static unsigned int s_KeyQueueReadIndex = 0;
static unsigned int cycle = 0;

static int old_direction = 0;
static int old_fire = 0;

static void addKeyToQueue(int pressed, unsigned int keyCode) {
  unsigned short keyData = (pressed << 8) | keyCode;

  s_KeyQueue[s_KeyQueueWriteIndex] = keyData;
  s_KeyQueueWriteIndex++;
  s_KeyQueueWriteIndex %= KEYQUEUE_SIZE;
}

void DG_Init() {
  memset(s_KeyQueue, 0, KEYQUEUE_SIZE * sizeof(unsigned short));
}

void DG_DrawFrame() {
  // Make sure we are in game
  addKeyToQueue(1, KEY_ENTER);
  addKeyToQueue(0, KEY_ENTER);

  // Controls by steering
  int inputs = CatalystAdaptor::Steer(cycle);
  if (inputs >= 0) {
    int direction = inputs % 5;
    int fire = inputs / 5;

    if (old_fire != fire) {
      addKeyToQueue(fire, KEY_FIRE);
    }

    if (old_direction != direction) {
      switch (direction) {
      case 1:
        addKeyToQueue(1, KEY_LEFTARROW);
        break;
      case 2:
        addKeyToQueue(1, KEY_RIGHTARROW);
        break;
      case 3:
        addKeyToQueue(1, KEY_UPARROW);
        break;
      case 4:
        addKeyToQueue(1, KEY_DOWNARROW);
        break;
      }

      switch (old_direction) {
      case 1:
        addKeyToQueue(0, KEY_LEFTARROW);
        break;
      case 2:
        addKeyToQueue(0, KEY_RIGHTARROW);
        break;
      case 3:
        addKeyToQueue(0, KEY_UPARROW);
        break;
      case 4:
        addKeyToQueue(0, KEY_DOWNARROW);
        break;
      }
    }

    old_direction = direction;
    old_fire = fire;
  }

  cycle++;
  CatalystAdaptor::Execute(cycle, DG_ScreenBuffer);
}

void DG_SleepMs(uint32_t ms) { usleep(ms * 1000); }

uint32_t DG_GetTicksMs() {
  struct timeval tp;
  struct timezone tzp;

  gettimeofday(&tp, &tzp);

  return (tp.tv_sec * 1000) + (tp.tv_usec / 1000);
}

int DG_GetKey(int *pressed, unsigned char *doomKey) {

  if (s_KeyQueueReadIndex == s_KeyQueueWriteIndex) {
    // key queue is empty
    return 0;
  } else {
    unsigned short keyData = s_KeyQueue[s_KeyQueueReadIndex];
    s_KeyQueueReadIndex++;
    s_KeyQueueReadIndex %= KEYQUEUE_SIZE;

    *pressed = keyData >> 8;
    *doomKey = keyData & 0xFF;

    return 1;
  }
}

void DG_SetWindowTitle(const char *title) {}

int main(int argc, char *argv[]) {
  CatalystAdaptor::Initialize(argc, argv);
  doomgeneric_Create(argc, argv);

  for (int i = 0;; i++) {
    doomgeneric_Tick();
  }

  return 0;
}