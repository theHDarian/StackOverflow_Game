enum BulletEffectType {
    BulletDamage,
    ProjectileSpeed,
    FireRate,
    BulletRange,
    Bounce,
    Pierce,
    PlayerSpeed,
    PlayerNumDash,
    PlayerStackSize,
    PlayerDashCDR,
    Regular //damaging bullet
};

struct BulletStackEffect {
	BulletEffectType type = BulletEffectType::Regular;
	int tier;
};