#include "BasicProjectile.h"
#include "ResourceManager.h"

#define MAX_PROJ 8192
#define MAX_PROJ_TYPE 256

Deque * free_projectile_ids;
Projectile projectiles[MAX_PROJ];
ProjectileTypeDescriptor projectile_types[MAX_PROJ_TYPE];

inline void add_type(short type, int x, int y, int w, int h, float originX, float originY, float pdr0, int colorId) {
	projectile_types[type].Region.x = x;
	projectile_types[type].Region.y = y;
	projectile_types[type].Region.w = w;
	projectile_types[type].Region.h = h;
	projectile_types[type].Origin.X = originX;
	projectile_types[type].Origin.Y = originY;
	projectile_types[type].JudgeRadius = pdr0;
	projectile_types[type].ColorID = colorId;
}

inline void add_types() {
	add_type(1, 0, 0, 12, 16, 6, 8, 2, 0);
	add_type(2, 20, 0, 12, 16, 6, 8, 2, 1);
	add_type(3, 40, 0, 12, 16, 6, 8, 2, 2);
	add_type(4, 60, 0, 12, 16, 6, 8, 2, 3);
	add_type(5, 80, 0, 12, 16, 6, 8, 2, 4);
	add_type(6, 100, 0, 12, 16, 6, 8, 2, 5);
	add_type(7, 120, 0, 12, 16, 6, 8, 2, 6);
	add_type(8, 140, 0, 12, 16, 6, 8, 2, 7);
	add_type(9, 0, 20, 18, 18, 9, 9, 3, 0);
	add_type(10, 20, 20, 18, 18, 9, 9, 3, 1);
	add_type(11, 40, 20, 18, 18, 9, 9, 3, 2);
	add_type(12, 60, 20, 18, 18, 9, 9, 3, 3);
	add_type(13, 80, 20, 18, 18, 9, 9, 3, 4);
	add_type(14, 100, 20, 18, 18, 9, 9, 3, 5);
	add_type(15, 120, 20, 18, 18, 9, 9, 3, 6);
	add_type(16, 140, 20, 18, 18, 9, 9, 3, 7);
	add_type(17, 0, 40, 16, 16, 8, 8, 3, 0);
	add_type(18, 20, 40, 16, 16, 8, 8, 3, 1);
	add_type(19, 40, 40, 16, 16, 8, 8, 3, 2);
	add_type(20, 60, 40, 16, 16, 8, 8, 3, 3);
	add_type(21, 80, 40, 16, 16, 8, 8, 3, 4);
	add_type(22, 100, 40, 16, 16, 8, 8, 3, 5);
	add_type(23, 120, 40, 16, 16, 8, 8, 3, 6);
	add_type(24, 140, 40, 16, 16, 8, 8, 3, 7);
	add_type(25, 0, 60, 10, 19, 5, 9, 2, 0);
	add_type(26, 10, 60, 10, 19, 5, 9, 2, 1);
	add_type(27, 20, 60, 10, 19, 5, 9, 2, 2);
	add_type(28, 30, 60, 10, 19, 5, 9, 2, 3);
	add_type(29, 40, 60, 10, 19, 5, 9, 2, 4);
	add_type(30, 50, 60, 10, 19, 5, 9, 2, 5);
	add_type(31, 60, 60, 10, 19, 5, 9, 2, 6);
	add_type(32, 70, 60, 10, 19, 5, 9, 2, 7);
	add_type(33, 80, 60, 8, 14, 4, 7, 2, 0);
	add_type(34, 90, 60, 8, 14, 4, 7, 2, 1);
	add_type(35, 100, 60, 8, 14, 4, 7, 2, 2);
	add_type(36, 110, 60, 8, 14, 4, 7, 2, 3);
	add_type(37, 120, 60, 8, 14, 4, 7, 2, 4);
	add_type(38, 130, 60, 8, 14, 4, 7, 2, 5);
	add_type(39, 140, 60, 8, 14, 4, 7, 2, 6);
	add_type(40, 150, 60, 8, 14, 4, 7, 2, 7);
	add_type(41, 0, 80, 10, 22, 5, 11, 3, 0);
	add_type(42, 20, 80, 10, 22, 5, 11, 3, 1);
	add_type(43, 40, 80, 10, 22, 5, 11, 3, 2);
	add_type(44, 60, 80, 10, 22, 5, 11, 3, 3);
	add_type(45, 80, 80, 10, 22, 5, 11, 3, 4);
	add_type(46, 100, 80, 10, 22, 5, 11, 3, 5);
	add_type(47, 120, 80, 10, 22, 5, 11, 3, 6);
	add_type(48, 140, 80, 10, 22, 5, 11, 3, 7);
	add_type(49, 0, 110, 12, 14, 6, 7, 3, 0);
	add_type(50, 20, 110, 12, 14, 6, 7, 3, 1);
	add_type(51, 40, 110, 12, 14, 6, 7, 3, 2);
	add_type(52, 60, 110, 12, 14, 6, 7, 3, 3);
	add_type(53, 80, 110, 12, 14, 6, 7, 3, 4);
	add_type(54, 100, 110, 12, 14, 6, 7, 3, 5);
	add_type(55, 120, 110, 12, 14, 6, 7, 3, 6);
	add_type(56, 140, 110, 12, 14, 6, 7, 3, 7);
	add_type(57, 0, 130, 9, 17, 5, 9, 3, 0);
	add_type(58, 10, 130, 9, 17, 5, 9, 3, 1);
	add_type(59, 20, 130, 9, 17, 5, 9, 3, 2);
	add_type(60, 30, 130, 9, 17, 5, 9, 3, 3);
	add_type(61, 40, 130, 9, 17, 5, 9, 3, 4);
	add_type(62, 50, 130, 9, 17, 5, 9, 3, 5);
	add_type(63, 60, 130, 9, 17, 5, 9, 3, 6);
	add_type(64, 70, 130, 9, 17, 5, 9, 3, 7);
	add_type(65, 80, 130, 16, 16, 8, 8, 4, 0);
	add_type(66, 100, 130, 16, 16, 8, 8, 4, 1);
	add_type(67, 120, 130, 16, 16, 8, 8, 4, 2);
	add_type(68, 140, 130, 16, 16, 8, 8, 4, 3);
	add_type(69, 160, 130, 16, 16, 8, 8, 4, 4);
	add_type(70, 180, 130, 16, 16, 8, 8, 4, 5);
	add_type(71, 200, 130, 16, 16, 8, 8, 4, 6);
	add_type(72, 220, 130, 16, 16, 8, 8, 4, 7);
	add_type(73, 0, 150, 8, 8, 4, 4, 3, 0);
	add_type(74, 10, 150, 8, 8, 4, 4, 3, 1);
	add_type(75, 20, 150, 8, 8, 4, 4, 3, 2);
	add_type(76, 30, 150, 8, 8, 4, 4, 3, 3);
	add_type(77, 40, 150, 8, 8, 4, 4, 3, 4);
	add_type(78, 50, 150, 8, 8, 4, 4, 3, 5);
	add_type(79, 60, 150, 8, 8, 4, 4, 3, 6);
	add_type(80, 70, 150, 8, 8, 4, 4, 3, 7);
	add_type(81, 80, 150, 8, 14, 4, 7, 3, 0);
	add_type(82, 90, 150, 8, 14, 4, 7, 3, 1);
	add_type(83, 100, 150, 8, 14, 4, 7, 3, 2);
	add_type(84, 110, 150, 8, 14, 4, 7, 3, 3);
	add_type(85, 120, 150, 8, 14, 4, 7, 3, 4);
	add_type(86, 130, 150, 8, 14, 4, 7, 3, 5);
	add_type(87, 140, 150, 8, 14, 4, 7, 3, 6);
	add_type(88, 150, 150, 8, 14, 4, 7, 3, 7);
	add_type(89, 160, 150, 8, 11, 3, 5, 2, 0);
	add_type(90, 170, 150, 8, 11, 3, 5, 2, 1);
	add_type(91, 180, 150, 8, 11, 3, 5, 2, 2);
	add_type(92, 190, 150, 8, 11, 3, 5, 2, 3);
	add_type(93, 200, 150, 8, 11, 3, 5, 2, 4);
	add_type(94, 210, 150, 8, 11, 3, 5, 2, 5);
	add_type(95, 220, 150, 8, 11, 3, 5, 2, 6);
	add_type(96, 230, 150, 8, 11, 3, 5, 2, 7);
	add_type(97, 0, 160, 10, 10, 5, 5, 3, 0);
	add_type(98, 10, 160, 10, 10, 5, 5, 3, 1);
	add_type(99, 20, 160, 10, 10, 5, 5, 3, 2);
	add_type(100, 30, 160, 10, 10, 5, 5, 3, 3);
	add_type(101, 40, 160, 10, 10, 5, 5, 3, 4);
	add_type(102, 50, 160, 10, 10, 5, 5, 3, 5);
	add_type(103, 60, 160, 10, 10, 5, 5, 3, 7);
	add_type(104, 0, 170, 32, 32, 16, 16, 5, 0);
	add_type(105, 40, 170, 32, 32, 16, 16, 5, 1);
	add_type(106, 80, 170, 32, 32, 16, 16, 5, 2);
	add_type(107, 120, 170, 32, 32, 16, 16, 5, 3);
	add_type(108, 160, 170, 32, 32, 16, 16, 5, 4);
	add_type(109, 200, 170, 32, 32, 16, 16, 5, 5);
	add_type(110, 240, 170, 32, 32, 16, 16, 5, 6);
	add_type(111, 280, 170, 32, 32, 16, 16, 5, 7);
	add_type(112, 0, 210, 31, 31, 15, 15, 8, 0);
	add_type(113, 40, 210, 31, 31, 15, 15, 8, 1);
	add_type(114, 80, 210, 31, 31, 15, 15, 8, 2);
	add_type(115, 120, 210, 31, 31, 15, 15, 8, 3);
	add_type(116, 160, 210, 31, 31, 15, 15, 8, 4);
	add_type(117, 200, 210, 31, 31, 15, 15, 8, 5);
	add_type(118, 240, 210, 31, 31, 15, 15, 8, 6);
	add_type(119, 280, 210, 31, 31, 15, 15, 8, 7);
	add_type(120, 0, 250, 38, 36, 19, 18, 5, 0);
	add_type(121, 50, 250, 38, 36, 19, 18, 5, 1);
	add_type(122, 100, 250, 38, 36, 19, 18, 5, 2);
	add_type(123, 150, 250, 38, 36, 19, 18, 5, 3);
	add_type(124, 200, 250, 38, 36, 19, 18, 5, 4);
	add_type(125, 250, 250, 38, 36, 19, 18, 5, 5);
	add_type(126, 300, 250, 38, 36, 19, 18, 5, 6);
	add_type(127, 350, 250, 38, 36, 19, 18, 5, 7);
	add_type(128, 0, 300, 24, 32, 12, 16, 4, 0);
	add_type(129, 30, 300, 24, 32, 12, 16, 4, 1);
	add_type(130, 60, 300, 24, 32, 12, 16, 4, 2);
	add_type(131, 90, 300, 24, 32, 12, 16, 4, 3);
	add_type(132, 120, 300, 24, 32, 12, 16, 4, 4);
	add_type(133, 150, 300, 24, 32, 12, 16, 4, 5);
	add_type(134, 180, 300, 24, 32, 12, 16, 4, 6);
	add_type(135, 210, 300, 24, 32, 12, 16, 4, 7);
	add_type(136, 240, 300, 20, 20, 10, 10, 3, 0);
	add_type(137, 260, 300, 20, 20, 10, 10, 3, 1);
	add_type(138, 280, 300, 20, 20, 10, 10, 3, 2);
	add_type(139, 300, 300, 20, 20, 10, 10, 3, 3);
	add_type(140, 320, 300, 20, 20, 10, 10, 3, 4);
	add_type(141, 340, 300, 20, 20, 10, 10, 3, 5);
	add_type(142, 360, 300, 20, 20, 10, 10, 3, 7);
	add_type(143, 0, 340, 15, 27, 7, 14, 4, 0);
	add_type(144, 20, 340, 15, 27, 7, 14, 4, 1);
	add_type(145, 40, 340, 15, 27, 7, 14, 4, 2);
	add_type(146, 60, 340, 15, 27, 7, 14, 4, 3);
	add_type(147, 80, 340, 15, 27, 7, 14, 4, 4);
	add_type(148, 100, 340, 15, 27, 7, 14, 4, 5);
	add_type(149, 120, 340, 15, 27, 7, 14, 4, 6);
	add_type(150, 140, 340, 15, 27, 7, 14, 4, 7);
	add_type(151, 0, 370, 11, 16, 5, 8, 3, 0);
	add_type(152, 20, 370, 11, 16, 5, 8, 3, 1);
	add_type(153, 40, 370, 11, 16, 5, 8, 3, 2);
	add_type(154, 60, 370, 11, 16, 5, 8, 3, 3);
	add_type(155, 80, 370, 11, 16, 5, 8, 3, 4);
	add_type(156, 100, 370, 11, 16, 5, 8, 3, 5);
	add_type(157, 120, 370, 11, 16, 5, 8, 3, 6);
	add_type(158, 140, 370, 11, 16, 5, 8, 3, 7);
	add_type(159, 0, 390, 31, 32, 15, 16, 10, 0);
	add_type(160, 40, 390, 31, 32, 15, 16, 10, 1);
	add_type(161, 80, 390, 31, 32, 15, 16, 10, 2);
	add_type(162, 120, 390, 31, 32, 15, 16, 10, 3);
	add_type(163, 160, 390, 31, 32, 15, 16, 10, 4);
	add_type(164, 200, 390, 31, 32, 15, 16, 10, 5);
	add_type(165, 240, 390, 31, 32, 15, 16, 10, 6);
	add_type(166, 280, 390, 31, 32, 15, 16, 10, 7);
	add_type(167, 320, 390, 12, 25, 6, 8, 3, 0);
	add_type(168, 335, 390, 12, 25, 6, 8, 3, 1);
	add_type(169, 350, 390, 12, 25, 6, 8, 3, 2);
	add_type(170, 365, 390, 12, 25, 6, 8, 3, 3);
	add_type(171, 381, 390, 12, 25, 6, 8, 3, 4);
	add_type(172, 397, 390, 12, 25, 6, 8, 3, 5);
	add_type(173, 413, 390, 12, 25, 6, 8, 3, 6);
	add_type(174, 170, 0, 63, 63, 32, 32, 10, 0);
	add_type(175, 240, 0, 63, 63, 32, 32, 10, 2);
	add_type(176, 310, 0, 63, 63, 32, 32, 10, 1);
	add_type(177, 170, 65, 63, 63, 32, 32, 10, 4);
	add_type(178, 240, 65, 63, 63, 32, 32, 10, 5);
	add_type(179, 310, 70, 35, 38, 17, 19, 0, -1);
	add_type(180, 310, 115, 35, 38, 17, 19, 0, -1);
	add_type(181, 380, 0, 33, 41, 17, 20, 6, 2);
	add_type(182, 380, 50, 33, 41, 17, 20, 6, 0);
	add_type(183, 380, 100, 33, 41, 17, 20, 6, 1);
	add_type(184, 380, 150, 33, 41, 17, 20, 6, 3);
	add_type(185, 380, 200, 33, 41, 17, 20, 6, 4);
	add_type(186, 240, 150, 17, 17, 8, 8, 5, 5);
	add_type(187, 260, 150, 17, 17, 8, 8, 5, 7);
	add_type(188, 280, 150, 17, 17, 8, 8, 5, 0);
	add_type(189, 330, 170, 36, 36, 18, 18, 10, 0);
	add_type(190, 330, 210, 36, 36, 18, 18, 10, 2);
	add_type(191, 7, 430, 78, 80, 39, 40, 20, -1);
	add_type(192, 90, 430, 58, 68, 29, 34, 20, -1);
	add_type(193, 160, 340, 36, 36, 18, 18, 9, -1);
	add_type(194, 200, 340, 36, 36, 18, 18, 9, -1);
	add_type(195, 240, 340, 36, 36, 18, 18, 9, -1);
	add_type(196, 280, 340, 36, 36, 18, 18, 9, -1);
	add_type(197, 320, 340, 36, 36, 18, 18, 9, -1);
	add_type(198, 360, 340, 36, 36, 18, 18, 9, -1);
	add_type(199, 150, 430, 50, 30, 25, 15, 5, -1);
	add_type(200, 210, 429, 63, 63, 31, 31, 15, -1);
	add_type(201, 280, 429, 63, 63, 31, 31, 15, -1);
	add_type(202, 350, 429, 63, 63, 31, 31, 15, -1);
	add_type(203, 0, 520, 7, 15, 3, 7, 2, 0);
	add_type(204, 10, 520, 7, 15, 3, 7, 2, 1);
	add_type(205, 20, 520, 7, 15, 3, 7, 2, 2);
	add_type(206, 30, 520, 7, 15, 3, 7, 2, 3);
	add_type(207, 40, 520, 7, 15, 3, 7, 2, 4);
	add_type(208, 50, 520, 7, 15, 3, 7, 2, 5);
	add_type(209, 60, 520, 7, 15, 3, 7, 2, 6);
	add_type(210, 70, 520, 7, 15, 3, 7, 2, 7);
	add_type(211, 90, 520, 12, 24, 6, 12, 2, 0);
	add_type(212, 110, 520, 12, 24, 6, 12, 2, 1);
	add_type(213, 130, 520, 12, 24, 6, 12, 2, 2);
	add_type(214, 150, 520, 12, 24, 6, 12, 2, 3);
	add_type(215, 170, 520, 12, 24, 6, 12, 2, 4);
	add_type(216, 190, 520, 12, 24, 6, 12, 2, 5);
	add_type(217, 210, 520, 12, 24, 6, 12, 2, 7);
	add_type(218, 225, 500, 60, 60, 30, 30, 10, -1);
	add_type(219, 295, 500, 60, 60, 30, 30, 10, -1);
	add_type(220, 365, 500, 60, 60, 30, 30, 10, -1);
	add_type(221, 0, 674, 64, 64, 32, 32, 10, 3);
	add_type(222, 64, 674, 64, 64, 32, 32, 10, 7);
	add_type(223, 0, 738, 64, 64, 32, 32, 10, 4);
	add_type(224, 64, 738, 64, 64, 32, 32, 10, 0);
	add_type(225, 0, 802, 64, 64, 32, 32, 10, 5);
	add_type(226, 64, 802, 64, 64, 32, 32, 10, 1);
	add_type(227, 0, 866, 64, 64, 32, 32, 10, 7);
	add_type(228, 64, 866, 64, 64, 32, 32, 10, 2);

}

inline void initialize_globals() {
	if (free_projectile_ids == NULL) {
		add_types();
		free_projectile_ids = create_deque(sizeof(int), 128u);
		for (int i = MAX_PROJ - 1; i >= 0; i--) {
			push_deque_tail(free_projectile_ids, &i);
		}
	}
}

void sync_proj_renderable(Projectile * proj) {
	if (proj->RenderablePt) {
		proj->RenderablePt->Center = proj->Position;
		proj->RenderablePt->Rotation = proj->Rotation;
		proj->RenderablePt->Scale = proj->Scale;
		proj->RenderablePt->Color = proj->Color;
	}
}

void update_basic_projectile(Projectile * proj) {
	if (proj->AI)
		proj->AI(proj);
	proj->Velocity = vector2_add(proj->Velocity, proj->Accel);
	proj->Position = vector2_add(proj->Position, proj->Velocity);
	sync_proj_renderable(proj);
}

Projectile * alloc_projectile() {
	initialize_globals();
	if (get_deque_size(free_projectile_ids) == 0) {
		SDL_Log("Error: Too Many Bullets!");
		return &projectiles[0];
	}
	Projectile * proj;
	int id;
	poll_deque_tail(free_projectile_ids, &id);
	proj = &projectiles[id];
	proj->AI = NULL;
	proj->RenderablePt = NULL;
	proj->Accel = proj->Velocity = proj->Position = vector2_zero();
	proj->Color = vector4_create(1.0f, 1.0f, 1.0f, 1.0f);
	proj->Scale = vector2_one();
	proj->Rotation = 0.0f;
	proj->Active = 1;
	proj->WhoAmI = id;
	return proj;
}

void raii_projectile_renderable(Projectile * proj) {
	proj->RenderablePt = malloc(sizeof(Renderable));
	proj->RenderablePt->Layer = RENDER_LAYER_ENTITY_5;
	proj->RenderablePt->TextureRegion.Rect = projectile_types[proj->Type].Region;
	proj->RenderablePt->TextureRegion.TextureID = resources.Images.Barrages;
	sync_proj_renderable(proj);
	register_renderable(proj->RenderablePt);
}

void free_projectile(Projectile * proj) {
	proj->Active = 0;
	unregister_renderable(proj->RenderablePt);
	free(proj->RenderablePt);
	proj->RenderablePt = NULL;
	push_deque_tail(free_projectile_ids, &proj->WhoAmI);
}

void update_projectiles() {
	for (int i = 0; i < MAX_PROJ; i++) {
		if (projectiles[i].Active) {
			update_basic_projectile(&projectiles[i]);
		}
	}
}

const int bullet_types[25][8] = {
	{ 1,2,3,4,5,6,7,8 }, // 0
	{ 9,10,11,12,13,14,15,16 }, // 1
	{ 17,18,19,20,21,22,23,24 }, // 2
	{ 25,26,27,28,29,30,31,32 }, // 3
	{ 33,34,35,36,37,38,39,40 }, // 4
	{ 41,42,43,44,45,46,47,48 }, // 5
	{ 73,74,75,76,77,78,79,80 }, // 6
	{ 49,50,51,52,53,54,55,56 }, // 7
	{ 57,58,59,60,61,62,63,64 }, // 8
	{ 65,66,67,68,69,70,71,72 }, // 9
	{ 81,82,83,84,85,86,87,88 }, // 10
	{ 89,90,91,92,93,94,95,96 }, // 11
	{ 136,137,138,139,140,141,142 }, // 12
	{ 97,98,99,100,101,102,103 }, // 13
	{ 104,105,106,107,108,109,110,111 }, // 14
	{ 112,113,114,115,116,117,118,119 }, // 15
	{ 120,121,122,123,124,125,126,127 }, // 16
	{ 128,129,130,131,132,133,134,135 }, // 17
	{ 143,144,145,146,147,148,149,150 }, // 18
	{ 151,152,153,154,155,156,157,158 }, // 19
	{ 159,160,161,162,163,164,165,166 }, // 20

	{ 167,168,169,170,171,172,173 }, // 21

	{ 203,204,205,206,207,208,209,210 }, // 22
	{ 210,211,212,213,214,215,216,217 }, // 23
	{ 221,222,223,224,225,226,227,228 } // 24
};
