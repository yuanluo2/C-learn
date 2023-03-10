#include <stdio.h>

/* Animal base class, and associated functions. */
struct Animal{
	char* type_name;
	void (*run) (struct Animal* a);
	void (*eat) (struct Animal* a);
};

void print_type_name(struct Animal* a) { printf("type is %s.\n", a->type_name); }
void run(struct Animal* a) { a->run(a); }
void eat(struct Animal* a) { a->eat(a); }

struct Dog{
	struct Animal a;   /* must be the 1st field. */
};

struct Cat{
	struct Animal a;   /* must be the 1st field. */
};

void dog_run(struct Animal* a){ printf("Dog is running.\n"); }
void dog_eat(struct Animal* a){ printf("Dog eats bones.\n"); }
void cat_run(struct Animal* a){ printf("Cat is running.\n"); }
void cat_eat(struct Animal* a){ printf("Cat eats fish.\n"); }

void dog_init(struct Dog* dog){
	dog->a.type_name = "dog";
	dog->a.run = dog_run;
	dog->a.eat = dog_eat;
}

void cat_init(struct Cat* cat){
	cat->a.type_name = "cat";
	cat->a.run = cat_run;
	cat->a.eat = cat_eat;
}

int main(){
	struct Dog d;
	dog_init(&d);
	
	struct Cat c;
	cat_init(&c);
	
	run((struct Animal*)&d);
	run((struct Animal*)&c);
	
	eat((struct Animal*)&d);
	eat((struct Animal*)&c);
	
	print_type_name((struct Animal*)&d);
	print_type_name((struct Animal*)&c);
}
