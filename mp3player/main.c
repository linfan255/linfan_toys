#include <stdio.h>
#include "player.h"
#include "net.h"

void showMenu(char *filename) {
	system("clear");
	printf("\n\n\n\n\n");
	printf("                                  0--exit\n");
	printf("                                1--play/stop\n");
	printf("                                  2--next\n");
	printf("                                  3--prev\n");
	printf("                          4--download song from server\n");
	printf("                                 5--song list\n");
	printf("\n");
	if(filename != NULL)
		printf("                        ==========playing %s===========\n", filename);
}

static void sig_alrm(int signo) {
	if(isPlaying)
	  kill(pid, SIGKILL);
	curr = curr->next;
	if(curr == NULL)	curr = head;
	Play(curr->songName);
}

int main(int argc, char **argv) {
	initList();
	curr = head;

	isPlaying = 0, init = 1;	//标识当前是否正在播放
	int songid;

	showMenu(NULL);
	for(;;) {
		if(signal(SIGALRM, sig_alrm) == SIG_ERR)
		  perror("signal alarm");
		op = -1;
		scanf("%d", &op);
		getchar();

		switch(op) {
			case 0:
				if(isPlaying)
				  kill(pid, SIGKILL);
				isPlaying = 0;
				return 0;

			case 1:
				if(init) {
					isPlaying = 1;
					init = 0;
					showMenu(curr->songName);
					Play(curr->songName);
				}
				else {
					if(isPlaying) {
						isPlaying = 0;
						kill(pid, SIGSTOP);
					}
					else {
						isPlaying = 1;
						kill(pid, SIGCONT);	
					}
				}
				break;

			case 2:
				if(isPlaying) {
					kill(pid, SIGKILL);
					isPlaying = 0;
				}
			
				isPlaying = 1;
				curr = curr->next;
				if(curr == NULL)	curr = head;
				showMenu(curr->songName);
				Play(curr->songName);
				break;

			case 3:
				if(isPlaying) {
					kill(pid, SIGKILL);
					isPlaying = 0;
				}
				
				isPlaying = 1;
				curr = curr->prev;
				if(curr == NULL)	curr = tail;
				showMenu(curr->songName);
				Play(curr->songName);
				break;
				
			case 4:
				showMenu(curr->songName);
				getSongFromServ();
				break;

			case 5:
				showMenu(curr->songName);
				print();
				break;

			case 6:
				printf("input the song id:");
				scanf("%d", &songid);
				getchar();
				while(curr->id != songid) {
					curr = curr->next;
					if(curr == NULL)	curr = head;
				}
				if(isPlaying) {
					kill(pid, SIGKILL);
					isPlaying = 0;
				}
				isPlaying = 1;
				Play(curr->songName);
				break;


			case 9:
				printf("pid2 = %d,pid = %d,isPlaying = %d,init = %d\n", pid2, pid, isPlaying, init);
				break;
			default:	break;
		}
	}
	destroy();
	return 0;
}
