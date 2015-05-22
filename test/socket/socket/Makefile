all: PeerA PeerB
PeerB: PeerB.c sendAndReceiveFunctions.c sendAndReceiveFunctions.h
	gcc -o PeerB PeerB.c sendAndReceiveFunctions.c sendAndReceiveFunctions.h
PeerA: PeerA.c sendAndReceiveFunctions.c sendAndReceiveFunctions.h
	gcc -o PeerA PeerA.c sendAndReceiveFunctions.c sendAndReceiveFunctions.h
clean:
	rm -rf PeerA PeerB

