enum {LEFT, MIDDLE, RIGHT};

void toh(int from, int to, int free, int number) {

  if (number == 0) return;

  toh(from, free, to, number -1);

  move(from, to);

  toh(free, to, from, number -1);

}


    
