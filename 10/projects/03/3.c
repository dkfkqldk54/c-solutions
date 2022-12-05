#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_RANKS 13
#define NUM_SUITS 4
#define NUM_CARDS 5

bool straight, flush, four, three;
int pairs;
int hands[NUM_CARDS][2];

void read_cards(void);
void analyze_hand(void);
void print_result(void);
bool duplicated_card(int rank, int suit, int cards_read, int hand[NUM_CARDS][2]);

int main(void)
{
  for (;;) {
    read_cards();
    analyze_hand();
    print_result();
  }
}

void read_cards(void)
{
  char ch, rank_ch, suit_ch;
  int rank, suit;
  bool bad_card;
  int cards_read = 0;

  while (cards_read < NUM_CARDS) {
    bad_card = false;

    printf("Enter a card: ");

    rank_ch = getchar();
    switch (rank_ch) {
      case '0': exit(EXIT_SUCCESS);
      case '2': rank = 0; break;
      case '3': rank = 1; break;
      case '4': rank = 2; break;
      case '5': rank = 3; break;
      case '6': rank = 4; break;
      case '7': rank = 5; break;
      case '8': rank = 6; break;
      case '9': rank = 7; break;
      case 't': case 'T': rank = 8; break;
      case 'j': case 'J': rank = 9; break;
      case 'q': case 'Q': rank = 10; break;
      case 'k': case 'K': rank = 11; break;
      case 'a': case 'A': rank = 12; break;
      default: bad_card = true;
    }

    suit_ch = getchar();
    switch (suit_ch) {
      case 'c': case 'C': suit = 0; break;
      case 'd': case 'D': suit = 1; break;
      case 'h': case 'H': suit = 2; break;
      case 's': case 'S': suit = 3; break;
      default: bad_card = true;
    }

    while ( (ch = getchar()) != '\n')
      if (ch != ' ') bad_card = true;

    if (bad_card)
      printf("Bad card; ignored.\n");
    else if (duplicated_card(rank, suit, cards_read, hands))
      printf("Duplicate card; ignored.\n");
    else {
      hands[cards_read][0] = rank;
      hands[cards_read][1] = suit;
      cards_read++;
    }
  }
}

bool duplicated_card(int rank, int suit, int cards_read, int hands[][2])
{
  for (int i=0; i < cards_read; i++) {
    if (hands[i][0] == rank && hands[i][1] == suit)
      return true;
  }
  return false;
}

void analyze_hand(void)
{
  int rank, suit, matches;

  straight = false;
  flush = false;
  four = false;
  three = false;
  pairs = 0;

  /* flush 확인 */
  int flush_check = 0;
  for (int i = 0; i < NUM_CARDS; i++)
    if (hands[i][1] == hands[0][1]) flush_check++;
  if (flush_check == NUM_CARDS) flush = true;

  /* 선택 정렬 */
  for (int i = 0; i < NUM_CARDS; i++) {
    int smallest = i;
    for (int j = i + 1; j < NUM_CARDS; j++) {
      if (hands[smallest][0] > hands[j][0])
        smallest = j;
    }
    int temp = hands[i][0];
    hands[i][0] = hands[smallest][0];
    hands[smallest][0] = temp;
    temp = hands[i][1];
    hands[i][1] = hands[smallest][1];
    hands[smallest][1] = temp;
  }
  
  /* straight */
  for (int i = 0; i < 9; i++) {
    bool checker = true;
    for (int j = 0; j < 5; j++) {
      if (hands[j][0] != i+j)
        checker = false;
    }
    if (checker) 
      straight = true;
  }  

  if (hands[0][0] == 0 && hands[1][0] == 9 && hands[2][0] == 10 && hands[3][0] == 11 && hands[4][0] == 12)
    straight = true;
  else if (hands[0][0] == 0 && hands[1][0] == 1 && hands[2][0] == 10 && hands[3][0] == 11 && hands[4][0] == 12)
    straight = true;
  else if (hands[0][0] == 0 && hands[1][0] == 1 && hands[2][0] == 2 && hands[3][0] == 11 && hands[4][0] == 12)
    straight = true;
  else if (hands[0][0] == 0 && hands[1][0] == 1 && hands[2][0] == 2 && hands[3][0] == 3 && hands[4][0] == 12)
    straight = true;

  /* four, three, pair*/

  int card = 0;
  while (card < NUM_CARDS) {
    rank = hands[card][0];
    matches = 0;
    do {
      card++;
      matches++;
    } while (card < NUM_CARDS && hands[card][0] == rank);
    switch (matches) {
      case 2: pairs++; break;
      case 3: three = true; break;
      case 4: four = true; break;
    }
  }
  
}

void print_result(void)
{
  if (straight && flush) printf("Straight flush");
  else if (four) printf("Four of a kind");
  else if (three && pairs == 1) printf("Full house");
  else if (flush) printf("Flush");
  else if (straight) printf("Straight");
  else if (three) printf("Three of a kind");
  else if (pairs == 2) printf("Two paris");
  else if (pairs == 1) printf("Pair");
  else printf("High card");

  printf("\n\n");
}
