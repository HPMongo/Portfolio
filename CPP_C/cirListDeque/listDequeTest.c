#include "cirListDeque.h"
#include <stdio.h>

int main(){
	struct cirListDeque* q = createCirListDeque(); 
	printf("Adding 1, 2, 3 to the back -\n");
	addBackCirListDeque(q, (TYPE)1);
	printf("%g\n", backCirListDeque(q));
	addBackCirListDeque(q, (TYPE)2);
	printf("%g\n", backCirListDeque(q));
	addBackCirListDeque(q, (TYPE)3);
	printf("%g\n", backCirListDeque(q));	
	
	printf("Adding 4, 5, 6 to the front -\n");
	addFrontCirListDeque(q, (TYPE)4);
	printf("%g\n", frontCirListDeque(q));
	addFrontCirListDeque(q, (TYPE)5);
 	printf("%g\n", frontCirListDeque(q));
	addFrontCirListDeque(q, (TYPE)6);
 	printf("%g\n", frontCirListDeque(q));

	printCirListDeque(q);
	printf("%g\n", frontCirListDeque(q));
	printf("%g\n", backCirListDeque(q));
	
	printf("Removing the front element - 6 -\n");	
	removeFrontCirListDeque(q);
	printf("%g\n", frontCirListDeque(q));	
	printf("Removing the back element - 3 -\n");		
	removeBackCirListDeque(q);
	printf("%g\n", backCirListDeque(q));	
	printCirListDeque(q);

	reverseCirListDeque(q);
	printCirListDeque(q);
	freeCirListDeque(q);

	return 0;
}
