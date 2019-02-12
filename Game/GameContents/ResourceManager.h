#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

struct Resources {
	struct {
		long Barrages;
	} Images;
	struct {
		long Biu;
	} SE;
	struct {
		long StartingScene;
	} BGM;
};

extern struct Resources resources;
#endif
