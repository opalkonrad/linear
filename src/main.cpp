#include <cstddef>
#include <cstdlib>
#include <chrono>
#include <string>
#include <iostream>

#define BIG 100000
#define MID 10000
#define SMALL 1000
#define TINY 100

#include "../include/Vector.h"
#include "../include/LinkedList.h"

using namespace aisdi;

void prependVector(int size) {
  Vector<int> v;

  for(int i = 0; i < size; i++) {
    v.append(i);
  }

  auto vStart = std::chrono::high_resolution_clock::now();
  v.prepend(5);
  auto vStop = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double> elapsed = vStop - vStart;
  std::cout << "Vector prepend() " << size << "+1 element is: " << elapsed.count()*1e6  << " microsecs\n";
}

void prependLinkedList(int size) {
  LinkedList<int> ll;

  for(int i = 0; i < size; i++) {
    ll.append(i);
  }

  auto llStart = std::chrono::high_resolution_clock::now();
  ll.prepend(5);
  auto llStop = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double> elapsed = llStop - llStart;
  std::cout << "Linked List prepend() " << size << "+1 element is: " << elapsed.count()*1e6  << " microsecs\n";
}


int main()
{
  prependVector(TINY);
  prependVector(SMALL);
  prependVector(MID);
  prependVector(BIG);

  prependLinkedList(TINY);
  prependLinkedList(SMALL);
  prependLinkedList(MID);
  prependLinkedList(BIG);

  return 0;
}
