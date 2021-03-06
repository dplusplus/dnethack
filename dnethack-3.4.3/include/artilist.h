/*	SCCS Id: @(#)artilist.h 3.4	2003/02/12	*/
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/* NetHack may be freely redistributed.  See license for details. */

#ifdef MAKEDEFS_C
/* in makedefs.c, all we care about is the list of names */

#define A(nam,typ,s1,s2,mt,atk,dfn,cry,inv,al,cl,rac,cost, s12, s22, ws) nam

static const char *artifact_names[] = {
#else
/* in artifact.c, set up the actual artifact list structure */

#define A(nam,typ,s1,s2,mt,atk,dfn,cry,inv,al,cl,rac,cost, s12, s22, ws) \
 { typ, nam, s1, s2, mt, atk, dfn, cry, inv, al, cl, rac, cost, s12, s22, ws }

#define     NO_ATTK	{0,0,0,0}		/* no attack */
#define     NO_DFNS	{0,0,0,0}		/* no defense */
#define     NO_CARY	{0,0,0,0}		/* no carry effects */
#define     DFNS(c)	{0,c,0,0}		//DFNS(AD_MAGM) == grants magic resistance when wielded
#define     CARY(c)	{0,c,0,0}		//CARY(AD_MAGM) == grants magic resistance when in open inventory
#define     PHYS(a,b)	{0,AD_PHYS,a,b}		/* physical */
#define     DRLI(a,b)	{0,AD_DRLI,a,b}		/* life drain */
#define     COLD(a,b)	{0,AD_COLD,a,b}
#define     FIRE(a,b)	{0,AD_FIRE,a,b}
#define     ELEC(a,b)	{0,AD_ELEC,a,b}		/* electrical shock */
#define     ACID(a,b)	{0,AD_ACID,a,b}
#define     STUN(a,b)	{0,AD_STUN,a,b}		/* magical attack */

STATIC_OVL NEARDATA struct artifact artilist[] = {
#endif	/* MAKEDEFS_C */

/* Artifact cost rationale:
 * 1.  The more useful the artifact, the better its cost.
 * 2.  Quest artifacts are highly valued.
 * 3.  Chaotic artifacts are inflated due to scarcity (and balance).
 */


/*  dummy element #0, so that all interesting indices are non-zero */
A("",				STRANGE_OBJECT,
	0, 0, 0, NO_ATTK, NO_DFNS, NO_CARY, 0, A_NONE, NON_PM, NON_PM, 0L, 0,0 ,0),

//////////////////////Crowning Gifts///////////////////////////////////////
A("Excalibur",			LONG_SWORD,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_SEEK|SPFX_DEFN|SPFX_INTEL|SPFX_SEARCH),0,0,
	PHYS(5,10),	DRLI(0,0),	NO_CARY,	
	0, A_LAWFUL, PM_KNIGHT, NON_PM, 4000L, 
	0,0,0),
/*
 *	Stormbringer only has a 2 because it can drain a level,
 *	providing 8 more.
 */
A("Stormbringer",		RUNESWORD,
	(SPFX_RESTR|SPFX_ATTK|SPFX_DEFN|SPFX_INTEL|SPFX_DRLI), 0, 0,
	DRLI(5,2),	DRLI(0,0),	NO_CARY,	
	0, A_CHAOTIC, NON_PM, NON_PM, 8000L, 
	SPFX2_BLDTHRST,0,0),
/*
 *	Two problems:  1) doesn't let trolls regenerate heads,
 *	2) doesn't give unusual message for 2-headed monsters (but
 *	allowing those at all causes more problems than worth the effort).
 */
A("Vorpal Blade",		LONG_SWORD,
	(SPFX_RESTR|SPFX_BEHEAD), 0, 0,
	PHYS(5,1),	NO_DFNS,	NO_CARY, /*Special code in weapon.c throws an extra die, so 2d8+2 vs small, 2d12+2 vs large*/	
	0, A_NEUTRAL, NON_PM, NON_PM, 4000L, 
	0,0,0),

/*//////////Namable Artifacts//////////*/
/*most namables can be twoweaponed, look in obj.h */

/*
 *	Orcrist and Sting have same alignment as elves.
 */
A("Orcrist",			ELVEN_BROADSWORD,
	(SPFX_WARN|SPFX_DFLAG2), 0, (M2_ORC|M2_DEMON), /* the old elves fought balrogs too. */
	PHYS(5,0),	NO_DFNS,	NO_CARY,	
	0, A_CHAOTIC, NON_PM, PM_ELF, 2000L, 
	0,0,0),

/*
 *	The combination of SPFX_WARN and M2_something on an artifact
 *	will trigger EWarn_of_mon for all monsters that have the appropriate
 *	M2_something flags.  In Sting's case it will trigger EWarn_of_mon
 *	for M2_ORC monsters.
 */
A("Sting",			ELVEN_DAGGER,
	(SPFX_WARN|SPFX_DFLAG2), 0, M2_ORC, /*note: some code in artifact.c also aplies the damage bonus to s_spiders */
	PHYS(5,0),	NO_DFNS,	NO_CARY,	
	0, A_CHAOTIC, NON_PM, PM_ELF, 800L, 
	0,0,0),

A("Grimtooth",			ORCISH_DAGGER, //needs quote
    (SPFX_WARN|SPFX_DFLAG2), 0, (M2_ELF|M2_HUMAN|M2_MINION),
	PHYS(5,0),	NO_DFNS,	NO_CARY,	
	0, A_CHAOTIC, NON_PM, PM_ORC, 300L, 
	0,0,0),

A("Slave to Armok",			DWARVISH_MATTOCK, /* Amber Evil that destroys other evils */ /*two handed, so no twoweaponing.*/
	(SPFX_DFLAG2), 0, (M2_ELF|M2_LORD|M2_PEACEFUL|M2_ORC), /*DF Dwarves can be a nasty lot.*/
	PHYS(5,0),	NO_DFNS,	NO_CARY,
	0, A_LAWFUL, NON_PM, PM_DWARF, 2500L, 
	SPFX2_BLDTHRST,0,0), /*attacks neutrals and pets*/

/*//////////The Banes//////////*/
/*banes can be twoweaponed, look in obj.h*/

A("Dragonlance",			LANCE, /*  */
	(SPFX_RESTR|SPFX_DCLAS|SPFX_REFLECT), 0, S_DRAGON, /* also makes a handy weapon for knights, since it can't break */
	PHYS(10,20),	NO_DFNS,	NO_CARY,				/* plus, reflection */
	0, A_NONE, NON_PM, NON_PM, 5000L, 
	0,0,0),

A("Demonbane",			SILVER_SABER,//needs quote
	(SPFX_RESTR|SPFX_DFLAG2|SPFX_WARN), 0, M2_DEMON, /* blocks summoning and is a silver saber. */
	PHYS(10,20),	NO_DFNS,	NO_CARY,			/* Plus, demons are nasty. */
	0, A_LAWFUL, NON_PM, NON_PM, 2500L, 
	SPFX2_NOCALL,0,0),

A("Werebane",			SILVER_SABER, //needs quote
	(SPFX_RESTR|SPFX_DFLAG2|SPFX_WARN), 0, M2_WERE, /* protects against lycathropy and is silver */
	PHYS(10,20),	DFNS(AD_WERE),	NO_CARY,			/*works against many demihumans, including */
	0, A_NONE, NON_PM, NON_PM, 1500L,					/*a few late game enemies */
	(SPFX2_DEMIBANE|SPFX2_NOWERE),0,0),

A("Giantslayer",		AXE, //needs quote
	(SPFX_RESTR|SPFX_DFLAG2|SPFX_WARN), 0, (M2_GIANT|M2_ROCKTHROW), /* deducts move from hit giants. */
	PHYS(10,20),	NO_DFNS,	NO_CARY,								/* also works vs a few late game enemies. */
	0, A_NONE, NON_PM, NON_PM, 2000L, 
	0,0,0),

A("Vampire Killer",			BULLWHIP, //needs quote
	(SPFX_RESTR|SPFX_DFLAG2), 0, (M2_UNDEAD|M2_DEMON|M2_WERE), /* some standard castlevainia enemy types*/
	PHYS(10,20),	DRLI(0,0),	NO_CARY,	/*is given extra damage in weapon.c, since whip damage is so low*/
	BLESS, A_LAWFUL, NON_PM, NON_PM, 2500L, /*Bless: no timeout, bless weapon, set fixed, repair erosion,*/
	SPFX2_NOWERE,0,0),									/* and raise enchantment to +3. */

A("Kingslayer",		STILETTO, //needs quote
	(SPFX_RESTR|SPFX_DFLAG2|SPFX_DISPL|SPFX_WARN), 0, (M2_LORD|M2_PRINCE), /* works against just about all the late game badies */
	PHYS(10,20),	NO_DFNS,	NO_CARY,	
	0, A_CHAOTIC, NON_PM, NON_PM, 2500L, 
	SPFX2_POISONED,0,0),

A("Peace Keeper",		ATHAME, 
	(SPFX_RESTR|SPFX_DFLAG2|SPFX_DISPL|SPFX_WARN), 0, (M2_HOSTILE), /* speaks for itself */
	PHYS(5,10),	NO_DFNS,	NO_CARY,	/*Weaker attack and damage */
	0, A_NONE, NON_PM, NON_PM, 2500L, 
	0,0,0),

A("Ogresmasher",		WAR_HAMMER, //needs quote
	(SPFX_RESTR|SPFX_DCLAS|SPFX_BEHEAD), 0, S_OGRE, //BEHEAD code smashes ogres
	PHYS(10,20),	NO_DFNS,	NO_CARY,			//set str to 25, and smashing ogres excercises str and wis
	0, A_NONE, NON_PM, NON_PM, 2000L, 				//overall maybe weakest of the banes
	0,0,0),

A("Trollsbane",			MORNING_STAR, //code early in attack petrifies trolls
	(SPFX_RESTR|SPFX_DFLAG1), 0, M1_REGEN, //sometimes gives bonus damage against monsters who pop in to ruin your day.
	PHYS(10,20),	NO_DFNS,	NO_CARY, //needs quote
	0, A_NONE, NON_PM, NON_PM, 2000L, 
	SPFX2_SILVERED|SPFX2_BRIGHT,0,0),//Silvered

/*//////////First Gifts//////////*/
//first gifts can be twoweaponed by their associated classes, look in obj.h

/*
 *	Mjollnir will return to the hand of the wielder when thrown
 *	if the wielder is a Valkyrie wearing Gauntlets of Power.
 *	
 *	Now it will never strike the Valkyrie or fall to the ground,
 *	if both are in good condition.
 */
A("Mjollnir",			WAR_HAMMER,		/* Mjo:llnir */
	(SPFX_RESTR|SPFX_ATTK),  0, 0,
	ELEC(5,24),	NO_DFNS,	NO_CARY,	
	0, A_NEUTRAL, PM_VALKYRIE, NON_PM, 4000L, 
	SPFX2_ELEC,0,0),

A("Cleaver",			BATTLE_AXE, /* quote... sorta */
	SPFX_RESTR, 0, 0,
	PHYS(3,0),	NO_DFNS,	NO_CARY,	
	0, A_NEUTRAL, PM_BARBARIAN, NON_PM, 1500L, 
	SPFX2_SHATTER,0,0),

/*	Need a way to convert era times to Japanese luni-solar months.*/
A("Kiku-ichimonji",		KATANA, /*quote (sorta) */
	SPFX_RESTR, 0, 0,
	PHYS(1,12),	NO_DFNS,	NO_CARY,
	0, A_LAWFUL, PM_SAMURAI, NON_PM, 1200L, /*Now this is the Samurai gift*/
	0,0,0), 

/* Clarent patch (Greyknight): Note that Clarent's SPFX2_DIG gives it another +2 to hit against thick-skinned
	monsters, as well as against wall-passers. Another special effect of Clarent
	is that it can be pulled out of a wall it is stuck in (by #untrapping towards
	it) if you are devoutly lawful. */
	/*Clarent has been modified to make it the Knight sac-gift*/
A("Clarent",			LONG_SWORD, /*quote (sorta)*/
	(SPFX_RESTR|SPFX_DEFN|SPFX_DFLAG1), 0, M1_THICK_HIDE,
	PHYS(4,8),	NO_DFNS,		NO_CARY,
	LEADERSHIP,	A_LAWFUL, PM_KNIGHT, NON_PM, 4000L, 
	SPFX2_DIG,0,0), 

/*
 *	Magicbane is a bit different!  Its magic fanfare
 *	unbalances victims in addition to doing some damage.
 */
A("Magicbane",			ATHAME,
	(SPFX_RESTR|SPFX_ATTK|SPFX_DEFN), 0, 0,//needs quote
	STUN(3,4),	DFNS(AD_MAGM),	NO_CARY,	
	0, A_NEUTRAL, PM_WIZARD, NON_PM, 3500L, 
	0,0,0),


/*//////////Double Damage Artifacts//////////*/

A("Grayswandir",		SILVER_SABER,
	(SPFX_RESTR|SPFX_HALRES|SPFX_WARN), 0, 0,
	PHYS(1, 0),	NO_DFNS,	NO_CARY,	
	0, A_LAWFUL, NON_PM, NON_PM, 8000L, 
	0,0,0),

A("Frost Brand",		LONG_SWORD, //needs quote
	(SPFX_RESTR|SPFX_ATTK|SPFX_DEFN), 0, 0,
	COLD(1,0),	COLD(0,0),	NO_CARY,	
	0, A_NONE, NON_PM, NON_PM, 3000L, 
	0,0,0),

A("Fire Brand",			LONG_SWORD,
	(SPFX_RESTR|SPFX_ATTK|SPFX_DEFN), 0, 0,
	FIRE(1,0),	FIRE(0,0),	NO_CARY,	
	0, A_NONE, NON_PM, NON_PM, 3000L, 
	0,0,0),

A("Mirror Brand",	LONG_SWORD,		//needs quote
	(SPFX_ATTK|SPFX_RESTR|SPFX_DALIGN|SPFX_REFLECT), 0,0,
	STUN(1,0),	NO_DFNS,	NO_CARY,
	0,	A_NEUTRAL, NON_PM, NON_PM, 3000L, 
	SPFX2_SILVERED,0,0),

A("Sunsword",			LONG_SWORD,	
	(SPFX_RESTR|SPFX_DFLAG2|SPFX_SEARCH|SPFX_ATTK|SPFX_BEHEAD), 0, (M2_UNDEAD|M2_DEMON),
	PHYS(1,0),	DFNS(AD_BLND),	NO_CARY,	/*also petrifies trolls, making this weapon strictly better than*/
	0, A_LAWFUL, NON_PM, NON_PM, 1500L, 	/*trollsbane.  But trollsbane can be twoweaponed.  And is silver.*/
	SPFX2_SILVERED|SPFX2_BLIND|SPFX2_BRIGHT,0,0), 

//A("The Axe of the Dwarvish Lords", BATTLE_AXE, /*can be thrown by dwarves*/
A("The Axe of the Dwarvish Lord", BATTLE_AXE, /*can be thrown by dwarves*/
	(SPFX_RESTR|SPFX_TCTRL|SPFX_XRAY), 0, 0,	/*x-ray vision is for dwarves only.*/
	PHYS(1, 0),	NO_DFNS,	NO_CARY, 
	0, A_LAWFUL, NON_PM, PM_DWARF, 4000L, //needs quote
	SPFX2_DIG,0,0),

A("Windrider",	BOOMERANG, /*returns to your hand.*/
	SPFX_RESTR, 0,0,
	PHYS(1,0),	NO_DFNS,	NO_CARY,//needs quote
	0,	A_NONE, NON_PM, NON_PM, 4000L, 
	0,0,0),

A("Rod of the Ram",			MACE, /* Wolf, Ram, and Hart? Ram demon? */
	(SPFX_RESTR), 0, 0,
	PHYS(1,0),	NO_DFNS,	NO_CARY, //needs quote	
	0, A_NEUTRAL, NON_PM, NON_PM, 3000L, 
	SPFX2_RAM2,0,0),

A("Tensa Zangetsu",		TSURUGI,
	(SPFX_RESTR|SPFX_HSPDAM), 0, 0, /*also has a haste effect when wielded, but massively increases hunger and damages the wielder*/
	PHYS(1,0),	NO_DFNS,	NO_CARY, /*The damage effect is cripplingly bad, really...*/
	SPEED_BANKAI, A_NEUTRAL, NON_PM, NON_PM, 4444L,/*The invoked attack is very powerful*/
	0,0,0), 

/*//////////Other Artifacts//////////*/

A("Sode no Shirayuki",		KATANA,
	(SPFX_RESTR|SPFX_ATTK), 0, 0,
	COLD(1,0),	COLD(0,0),	NO_CARY,  /*Sort of intermediate between a double damage and a utility weapon,*/
	ICE_SHIKAI, A_LAWFUL, NON_PM, NON_PM, 8000L,/*Sode no Shirayuki gains x2 ice damage after using the third dance.*/
	SPFX2_SILVERED,0,0), /*however, it only keeps it for a few rounds, and the other dances are attack magic. */

A("Tobiume",		LONG_SWORD,
	(SPFX_RESTR|SPFX_ATTK), 0, 0,
	FIRE(1,1),	FIRE(0,0),	NO_CARY,/*Tobiume is an awkward weapon.  It looses 3 damage vs large and 2 vs small*/
	FIRE_SHIKAI, A_CHAOTIC, NON_PM, NON_PM, 8000L,
	SPFX2_DISARM|SPFX2_FIRE2|SPFX2_RAM2,0,0),/*Ram and Fire blast only trigger if enemy is low hp*/

//A("Lance of Longinus",			SILVER_SPEAR,
A("Lancea Longini",			SILVER_SPEAR,
	(SPFX_RESTR|SPFX_HSPDAM|SPFX_HPHDAM|SPFX_DEFN), 0, 0, //needs quote
	NO_ATTK,	DFNS(AD_MAGM),	DRLI(0,0),
	WWALKING, A_LAWFUL, NON_PM, NON_PM, 1500L, 
	0,SPFX3_ENGRV,0),

A("The Arkenstone", DIAMOND,  /*by Aardvark Joe*/
    SPFX_RESTR, (SPFX_CONFL|SPFX_REFLECT|SPFX_AGGRM), 0,//needs quote
    NO_ATTK, NO_DFNS, NO_CARY, 
	0, A_CHAOTIC, NON_PM, NON_PM, 8000L, 
	0,0,0),

A("The Silence Glaive",		GLAIVE,
	(SPFX_RESTR|SPFX_ATTK|SPFX_DEFN|SPFX_DRLI), 0, 0, //needs quote
	DRLI(1,1),	DRLI(0,0),	NO_CARY,
	SATURN, A_NONE, NON_PM, NON_PM, 8000L, 
	0,0,0),

A("The Garnet Rod",		UNIVERSAL_KEY, //needs quote
	(SPFX_RESTR|SPFX_EREGEN|SPFX_REGEN), 0, 0, /*also has a haste effect when wielded, but massively increases hunger*/
	NO_ATTK,	NO_ATTK,	NO_CARY,	
	PLUTO, A_NONE, NON_PM, NON_PM, 8000L, 
	0,0,0),

A("Helping Hand",			GRAPPLING_HOOK, //needs quote
	(SPFX_RESTR|SPFX_SEEK|SPFX_SEARCH|SPFX_WARN),0,0,
	NO_ATTK,	NO_DFNS,	NO_CARY,
	UNTRAP, A_LAWFUL, NON_PM, NON_PM, 2000L, 
	SPFX2_STLTH,SPFX3_ENGRV,0),

A("Blade Singer's Spear",		SILVER_SPEAR,//needs quote
	(SPFX_RESTR|SPFX_DEFN), 0, 0,
	PHYS(6,6),	NO_DFNS,	NO_CARY,
	DANCE_DAGGER, A_NONE, NON_PM, NON_PM, 1500L, 
	SPFX2_DANCER,0,0),

A("Blade Dancer's Dagger",		SILVER_DAGGER,//needs quote
	(SPFX_RESTR|SPFX_DEFN), 0, 0,
	PHYS(4,4),	NO_DFNS,	NO_CARY,
	SING_SPEAR, A_NONE, NON_PM, NON_PM, 1500L, 
	SPFX2_DANCER,SPFX3_NOCNT,0),

A("Limb of the Black Tree",			CLUB,//needs quote
	(SPFX_RESTR|SPFX_ATTK|SPFX_DEFN), 0, 0,
	FIRE(4,1),	FIRE(0,0),	NO_CARY,
	0, A_CHAOTIC, NON_PM, NON_PM, 3000L, 
	SPFX2_FIRE2,0,0),

A("Hellfire",			CROSSBOW,/*adapted from Slash'em*/
	(SPFX_RESTR|SPFX_ATTK), 0, 0, /* hellfire song? */
	FIRE(4,1),	FIRE(0,0),	NO_CARY,
	0, A_CHAOTIC, NON_PM, NON_PM, 3000L, 
	SPFX2_FIRE2,0,0),

A("Lash of the Cold Waste",		BULLWHIP,
	(SPFX_RESTR|SPFX_ATTK|SPFX_DEFN), 0, 0,
	COLD(4,1),	COLD(0,0),	NO_CARY,	
	0, A_CHAOTIC, NON_PM, NON_PM, 3000L, 
	SPFX2_COLD2,0,0),

A("Ramiel",			PARTISAN, /* Greg Landsman */
	(SPFX_RESTR|SPFX_ATTK), 0, 0,
	ELEC(4,1),	NO_DFNS,	NO_CARY,	/*Ramiel's ranged attack is far more useful than the lash and the limb*/
	0, A_LAWFUL, NON_PM, NON_PM, 3000L, /*So it's your job to use it right!*/
	SPFX2_ELEC2,0,0),

A("Fuma-itto no Ken",		BROADSWORD, //needs quote
	(SPFX_RESTR|SPFX_DALIGN), 0, 0,   
	PHYS(1,8),	NO_DFNS,	NO_CARY,
	CREATE_AMMO, A_CHAOTIC, NON_PM, NON_PM, 1200L,/*makes shuriken*/
	SPFX2_POISONED|SPFX2_NINJA,0,0), 
/*  The name is meant to translate to "Sword of the Fuma clan,"
 *  "Ken" being a word for "sword" (as in "Bokken," "wooden sword")
 *  "no" being "of", and "Fuma-itto" being "Fuma-clan" (or so I hope...),
 *  this being a clan of ninja from the Sengoku era
 *  (there should be an accent over the u of "Fuma" and the o of "-itto").
 *  Nethack Samurai call broadswords "Ninja-to," which is the steriotypical ninja sword.
 *  Aparently, there was no such thing as an actual Ninja-to, its something Hollywood made up!
 */
A("Yoichi no yumi", YUMI, //needs quote
	(SPFX_RESTR), 0, 0,
	PHYS(20,0),	NO_DFNS,	NO_CARY,
	CREATE_AMMO, A_LAWFUL, NON_PM, NON_PM, 4000L, 
	0,0,0),

/*//////////Artifact Armors//////////*/

#ifdef TOURIST
A("Tie-Dye Shirt of Shambhala",			T_SHIRT, //needs quote
	(SPFX_RESTR), 0, 0,
	NO_ATTK,	NO_DFNS,	NO_CARY,
	ENLIGHTENING, A_NEUTRAL, NON_PM, NON_PM, 4500L, 
	0,SPFX3_MANDALA,(WSFX_LIGHTEN|WSFX_WCATRIB|WSFX_PLUSSEV)),
#endif
A("Grandmaster's Robe",			ROBE, /*double robe effect*/
	(SPFX_RESTR), 0, 0,/*martial arts attacks use exploding dice and get extra damage*/
	NO_ATTK,	NO_DFNS,	NO_CARY,
	0, A_NEUTRAL, NON_PM, NON_PM, 4500L, 
	0,0,(WSFX_PLUSSEV)),

A("Platinum Dragon Plate",			SILVER_DRAGON_SCALE_MAIL,/*heavier than normal, and causes spell penalties*/
	(SPFX_RESTR), 0, 0, //needs quote
	NO_ATTK,	DFNS(AD_MAGM),	NO_CARY,
	0, A_LAWFUL, NON_PM, NON_PM, 9000L, 
	0,0,0),

A("Beastmaster's Duster",			LEATHER_JACKET, //needs quote
	(SPFX_RESTR), 0, 0,
	NO_ATTK,	NO_DFNS,	NO_CARY,
	PETMASTER, A_LAWFUL, NON_PM, NON_PM, 9000L, 
	0,0,WSFX_PLUSSEV),

A("Mirrorbright",			SHIELD_OF_REFLECTION,/*adapted from Slash'em*/
	(SPFX_RESTR|SPFX_HALRES), 0, 0, //needs quote
	NO_ATTK,	NO_DFNS,	NO_CARY,
	CONFLICT, A_CHAOTIC, NON_PM, NON_PM, 4000L, 
	0,0,WSFX_PLUSSEV),

A("Whisperfeet",			SPEED_BOOTS,/*adapted from Slash'em*/
	(SPFX_RESTR), 0, 0, //needs quote
	NO_ATTK,	NO_DFNS,	NO_CARY,
	INVIS, A_CHAOTIC, NON_PM, NON_PM, 4000L, 
	SPFX2_STLTH,0,WSFX_PLUSSEV),

A("Shield of the Resolute Heart",		GAUNTLETS_OF_DEXTERITY,
	(SPFX_RESTR|SPFX_HPHDAM), 0, 0, //needs quote
	NO_ATTK,	NO_DFNS,	NO_CARY,
	BLESS, A_NONE, NON_PM, NON_PM, 4000L, 
	0,0,WSFX_PLUSSEV),

A("Gauntlets of Spell Power",		GAUNTLETS_OF_POWER, /*Note: it is quite deliberate that these cause */
	(SPFX_RESTR|SPFX_HSPDAM), 0, 0,						/*		a spellcasting penalty. */
	NO_ATTK,	NO_DFNS,	NO_CARY, //needs quote
	0, A_NONE, NON_PM, NON_PM, 4000L, 
	SPFX2_SILVERED|SPFX2_SPELLUP,0,0),

/*	Doesn't Work...
A("Hellrider's Saddle",			SADDLE,
	(SPFX_RESTR), SPFX_REFLECT, 0,
	NO_ATTK,	NO_DFNS,	CARY(AD_MAGM),
	INVIS, A_NONE, NON_PM, NON_PM, 4000L, 
	0,0),
 */
	//A("Shield of the All-Seeing", ORCISH_SHIELD,
//	(SPFX_RESTR), (SPFX_SEEK|SPFX_SEARCH|SPFX_WARN), M2_ELF,
//     NO_ATTK, NO_DFNS, CARY(AD_FIRE),
//	 PROT_FROM_SHAPE_CHANGERS, A_NONE, NON_PM, NON_PM, 3000L ),

	//A("Grond", WAR_HAMMER,
//  (SPFX_RESTR), 0, 0,
//  (), NO_DFNS, NO_CARY, 0, A_CHAOTIC, NON_PM, NON_PM, 1500L),

/*
 *	The artifacts for the quest dungeon, all self-willed.
 */

/*//////////Law Quest Artifacts//////////*/

A("The Rod of Seven Parts",	SILVER_SPEAR, /*From D&D*/
	(SPFX_ATTK|SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_DALIGN|SPFX_DEFN), 0,0,
	PHYS(7,20),	DRLI(0,0),	NO_CARY,
	SEVENFOLD,	A_LAWFUL, NON_PM, NON_PM, 7777L, 
	0,0,0),

A("Field Marshal's Baton",	MACE, //needs quote
	(SPFX_NOGEN|SPFX_RESTR), SPFX_WARN, M2_MERC,
	NO_ATTK,	NO_DFNS,	NO_CARY,
	CANNONADE,	A_LAWFUL, NON_PM, NON_PM, 5000L, 
	0,SPFX3_NOCNT,0),

/*//////////Chaos Quest Artifacts//////////*/

A("Werebuster",			LONG_SWORD, //needs quote
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_DFLAG2), 0, M2_WERE,/*should not be gifted or gened randomly*/
	PHYS(10,20),	NO_DFNS,	NO_CARY,
	0, A_NONE, NON_PM, NON_PM, 1500L, /*does not protect agains lycathropy*/
	0,SPFX3_NOCNT,0), /*does not count against artifacts generated*/

A("Masamune",			TSURUGI, //needs quote
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL), 0, 0,/*should not be gifted or gened randomly*/
	NO_ATTK,	NO_DFNS,	NO_CARY,
	BLESS, A_NONE, NON_PM, NON_PM, 7500L,
	SPFX2_SILVERED,SPFX3_NOCNT,0), /*does not count against artifacts generated*/

A("The Black Crystal", CRYSTAL_BALL, /*from Final Fantasy*/
        (SPFX_NOGEN|SPFX_RESTR|SPFX_DEFN|SPFX_DALIGN|SPFX_WARN), 0, 0, //needs quote
        PHYS(3,0), NO_DFNS, CARY(AD_MAGM), 
		SHADOW_FLARE, A_CHAOTIC, NON_PM, NON_PM, 100L, 
		0,0,0), /*The crystals are generated together.  The Black Crystal counts, and the others don't.*/

A("The Water Crystal", CRYSTAL_BALL,
        (SPFX_NOGEN|SPFX_RESTR|SPFX_DEFN), 0, 0, //needs quote
        NO_ATTK, NO_DFNS, COLD(0,0), 
		BLIZAGA, A_NONE, NON_PM, NON_PM, 100L, 
		0,SPFX3_NOCNT,0), /*does not count against artifacts generated*/

A("The Fire Crystal", CRYSTAL_BALL,
        (SPFX_NOGEN|SPFX_RESTR|SPFX_DEFN), 0, 0, //needs quote
        NO_ATTK, NO_DFNS, FIRE(0,0), 
		FIRAGA, A_NONE, NON_PM, NON_PM, 100L, 
		0,SPFX3_NOCNT,0),

A("The Earth Crystal", CRYSTAL_BALL,
        (SPFX_NOGEN|SPFX_RESTR|SPFX_DEFN), SPFX_HPHDAM, 0, //needs quote
        NO_ATTK, NO_DFNS, NO_CARY, 
		QUAKE, A_NONE, NON_PM, NON_PM, 100L, 
		0,SPFX3_NOCNT,0),

A("The Air Crystal", CRYSTAL_BALL,
        (SPFX_NOGEN|SPFX_RESTR|SPFX_DEFN), 0, 0, //needs quote
        NO_ATTK, NO_DFNS, ELEC(0,0), 
		THUNDAGA, A_NONE, NON_PM, NON_PM, 100L, 
		0,SPFX3_NOCNT,0),

A("Nighthorn",	UNICORN_HORN, /*from SLASH'EM, although modified from its original form*/
	(SPFX_NOGEN|SPFX_RESTR|SPFX_ATTK|SPFX_INTEL), SPFX_LUCK,0, //needs quote
	FIRE(12,24),	FIRE(0,0),	NO_CARY,
	LEVITATION,	A_CHAOTIC, NON_PM, NON_PM, 5000L, 
	SPFX2_FIRE2,SPFX3_FEAR,0), /*Fire explosion and acts as a scroll of scare monster.*/


/*//////Artifact Keys.  Must be grouped together.  Some code in lock.c depends on the order.  Also artifact.h//////*/

A("The First Key of Law", SKELETON_KEY, /*must be first*/
	(SPFX_NOGEN|SPFX_RESTR), 0, 0,
	NO_ATTK,	NO_DFNS,	NO_CARY,
	0,		A_LAWFUL, NON_PM, NON_PM, 1500L, 
	0,SPFX3_NOCNT,0),  /*None of the keys count against generated artifacts.*/

A("The Second Key of Law", SKELETON_KEY,
	(SPFX_NOGEN|SPFX_RESTR), 0, 0,
	NO_ATTK,	NO_DFNS,	NO_CARY,
	0,		A_LAWFUL, NON_PM, NON_PM, 1500L, 
	0,SPFX3_NOCNT,0),

A("The Third Key of Law", SKELETON_KEY,
	(SPFX_NOGEN|SPFX_RESTR), 0, 0,
	NO_ATTK,	NO_DFNS,	NO_CARY,
	0,		A_LAWFUL, NON_PM, NON_PM, 1500L, 
	0,SPFX3_NOCNT,0),

A("The First Key of Chaos", SKELETON_KEY,
	(SPFX_NOGEN|SPFX_RESTR), 0, 0,
	NO_ATTK,	NO_DFNS,	NO_CARY,
	0,		A_CHAOTIC, NON_PM, NON_PM, 1500L, 
	0,SPFX3_NOCNT,0),

A("The Second Key of Chaos", SKELETON_KEY,
	(SPFX_NOGEN|SPFX_RESTR), 0, 0,
	NO_ATTK,	NO_DFNS,	NO_CARY,
	0,		A_CHAOTIC, NON_PM, NON_PM, 1500L, 
	0,SPFX3_NOCNT,0),

A("The Third Key of Chaos", SKELETON_KEY,
	(SPFX_NOGEN|SPFX_RESTR), 0, 0,
	NO_ATTK,	NO_DFNS,	NO_CARY,
	0,		A_CHAOTIC, NON_PM, NON_PM, 1500L, 
	0,SPFX3_NOCNT,0),

A("The First Key of Neutrality", SKELETON_KEY,
	(SPFX_NOGEN|SPFX_RESTR), 0, 0,
	NO_ATTK,	NO_DFNS,	NO_CARY,
	0,		A_NEUTRAL, NON_PM, NON_PM, 1500L, 
	0,SPFX3_NOCNT,0),

A("The Second Key of Neutrality", SKELETON_KEY,
	(SPFX_NOGEN|SPFX_RESTR), 0, 0,
	NO_ATTK,	NO_DFNS,	NO_CARY,
	0,		A_NEUTRAL, NON_PM, NON_PM, 1500L, 
	0,SPFX3_NOCNT,0),

A("The Third Key of Neutrality", SKELETON_KEY, /*must be last*/
	(SPFX_NOGEN|SPFX_RESTR), 0, 0,
	NO_ATTK,	NO_DFNS,	NO_CARY,
	0,		A_NEUTRAL, NON_PM, NON_PM, 1500L, 
	0,SPFX3_NOCNT,0),

/*//////////Neutral Quest Artifacts//////////*/

A("The Necronomicon", SPE_SECRETS, /*from the works of HP Lovecraft*/
	(SPFX_NOGEN|SPFX_RESTR), 0,0, //needs quote
	NO_ATTK,	NO_DFNS,	NO_CARY,
	NECRONOMICON,	A_NONE, NON_PM, NON_PM, 5000L, 
	0,0,0),  /*polymorph control*/

A("The Silver Key", UNIVERSAL_KEY, /*from the works of HP Lovecraft*/
	(SPFX_NOGEN|SPFX_RESTR), (SPFX_EREGEN|SPFX_SEARCH|SPFX_TCTRL),0,
	NO_ATTK,	NO_DFNS,	CARY(AD_MAGM), //needs quote
	CREATE_PORTAL,	A_NEUTRAL, NON_PM, NON_PM, 5000L, 
	0,SPFX3_PCTRL,0),  /*polymorph control*/



/*//////////Role-specific Quest Artifacts//////////*/
/*
A("The Orb of Detection",	CRYSTAL_BALL,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL), (SPFX_ESP|SPFX_HSPDAM), 0,
	NO_ATTK,	NO_DFNS,	CARY(AD_MAGM),
	INVIS,		A_LAWFUL, PM_ARCHEOLOGIST, NON_PM, 2500L ),
*/
A("Itlachiayaque", SHIELD_OF_REFLECTION,/*From archeologist patch*/
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL), (SPFX_ESP|SPFX_HSPDAM), 0,
     NO_ATTK, NO_DFNS, CARY(AD_FIRE),
	 SMOKE_CLOUD, A_LAWFUL, PM_ARCHEOLOGIST, NON_PM, 3000L, 
	0,0,0),

A("The Heart of Ahriman",	LUCKSTONE,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_EREGEN|SPFX_INTEL), SPFX_DISPL|SPFX_HSPDAM, 0,
	/* this stone does double damage if used as a projectile weapon */
	PHYS(10,0),	NO_DFNS,	NO_CARY,
	LEVITATION,	A_NEUTRAL, PM_BARBARIAN, NON_PM, 2500L, 
	0,0,0),

A("The Sceptre of Might",	MACE,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_DALIGN), 0, 0,
	PHYS(5,0),	NO_DFNS,	CARY(AD_MAGM),
	CONFLICT,	A_LAWFUL, PM_CAVEMAN, NON_PM, 2500L, 
	SPFX2_RAM,0,0),

#if 0	/* OBSOLETE */
A("The Palantir of Westernesse",	CRYSTAL_BALL,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL),
		(SPFX_ESP|SPFX_REGEN|SPFX_HSPDAM), 0,
	NO_ATTK,	NO_DFNS,	NO_CARY,
	TAMING,		A_CHAOTIC, NON_PM , PM_ELF, 8000L ),
#endif

A("The Staff of Aesculapius",	QUARTERSTAFF,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_ATTK|SPFX_INTEL|SPFX_DRLI|SPFX_REGEN), 0,0,
	DRLI(1,0),	DRLI(0,0),	NO_CARY,
	HEALING,	A_NEUTRAL, PM_HEALER, NON_PM, 5000L, 
	SPFX2_SILVERED,0,0), /*silver is associated with the healer's art*/

A("The Magic Mirror of Merlin", MIRROR,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_SPEAK), SPFX_ESP, 0,
	NO_ATTK,	NO_DFNS,	CARY(AD_MAGM),
	0,		A_LAWFUL, PM_KNIGHT, NON_PM, 1500L, 
	0,0,0),

A("The Eyes of the Overworld",	LENSES,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_XRAY), 0, 0,
	NO_ATTK,	NO_DFNS,	CARY(AD_MAGM),
	ENLIGHTENING,	A_NEUTRAL,	 PM_MONK, NON_PM, 2500L, 
	0,0,0),

A("The Mitre of Holiness",	HELM_OF_BRILLIANCE,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_DFLAG2|SPFX_INTEL), 0, M2_UNDEAD,
	NO_ATTK,	NO_DFNS,	CARY(AD_FIRE),
	ENERGY_BOOST,	A_LAWFUL, PM_PRIEST, NON_PM, 2000L, 
	0,0,0),

A("The Longbow of Diana", BOW,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_REFLECT), SPFX_ESP, 0,
	PHYS(5,0),	NO_DFNS,	NO_CARY,
	CREATE_AMMO, A_CHAOTIC, PM_RANGER, NON_PM, 4000L, 
	SPFX2_SILVERED,0,0), /*silver is the moon's metal... bows don't enter this code...*/

A("The Master Key of Thievery", SKELETON_KEY,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_SPEAK),
		(SPFX_WARN|SPFX_TCTRL|SPFX_HPHDAM), 0,
	NO_ATTK,	NO_DFNS,	NO_CARY,
	UNTRAP,		A_CHAOTIC, PM_ROGUE, NON_PM, 3500L, 
	0,0,0),

A("The Tsurugi of Muramasa",	TSURUGI,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_SEEK|SPFX_INTEL|SPFX_BEHEAD|SPFX_LUCK), 0, 0,
	PHYS(2,0),	NO_DFNS,	NO_CARY,
	0,		A_LAWFUL, PM_SAMURAI, NON_PM, 4500L, 
	SPFX2_SHATTER|SPFX2_BLDTHRST,0,0),

#ifdef TOURIST
A("The Platinum Yendorian Express Card", CREDIT_CARD,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_DEFN),
		(SPFX_ESP|SPFX_HSPDAM), 0,
	NO_ATTK,	NO_DFNS,	CARY(AD_MAGM),
	CHARGE_OBJ,	A_NEUTRAL, PM_TOURIST, NON_PM, 7000L, 
	SPFX2_SILVERED,0,0), /*decorative silver rim.  Probably doesn't work...*/
#endif

A("The Orb of Fate",		CRYSTAL_BALL,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_LUCK),
		(SPFX_WARN|SPFX_HSPDAM|SPFX_HPHDAM), 0,
	NO_ATTK,	NO_DFNS,	NO_CARY,
	LEV_TELE,	A_NEUTRAL, PM_VALKYRIE, NON_PM, 3500L, 
	0,0,0),

A("The Eye of the Aethiopica",	AMULET_OF_ESP,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL), (SPFX_EREGEN|SPFX_HSPDAM), 0,
	NO_ATTK,	NO_DFNS,	CARY(AD_MAGM),
	CREATE_PORTAL,	A_NEUTRAL, PM_WIZARD, NON_PM, 4000L, 
	0,0,0),


/*//////////Special High-Level Artifacts//////////*/

A("Staff of the Archmagi",			QUARTERSTAFF,
	(SPFX_RESTR|SPFX_ATTK|SPFX_DEFN|SPFX_SEARCH|SPFX_LUCK|SPFX_DISPL|SPFX_INTEL), 0, 0,
	STUN(20,4),	DFNS(AD_MAGM),	NO_CARY,	
	ENERGY_BOOST, A_NONE, PM_WIZARD, NON_PM, 9000L, 
	SPFX2_DIG|SPFX2_FIRE|SPFX2_COLD|SPFX2_ELEC|SPFX2_RAM2,0,0),

A("Robe of the Archmagi",			ROBE,
	(SPFX_RESTR|SPFX_REFLECT|SPFX_INTEL), 0, 0,
	NO_ATTK,	NO_DFNS,	NO_CARY,	
	ENERGY_BOOST, A_NONE, PM_WIZARD, NON_PM, 9000L, 
	0,0,WSFX_PLUSSEV),

A("Hat of the Archmagi",			CORNUTHAUM,
	(SPFX_RESTR|SPFX_SPEAK|SPFX_WARN|SPFX_XRAY|SPFX_INTEL), 0, 0,
	NO_ATTK,	NO_DFNS,	NO_CARY,	
	ENERGY_BOOST, A_NONE, PM_WIZARD, NON_PM, 9000L, 
	0,0,0),
/* 22+ Daimyo can name Kusanagi no Tsurugi from a longsword 
 * (should be a broadsword maybe, but that has been "translated" as a ninja-to).
 * only a level 30 (Shogun) samurai or one who is carying the amulet can wield the sword.
 */
A("The Kusanagi no Tsurugi",	LONG_SWORD,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_EREGEN|SPFX_BEHEAD), (SPFX_SEARCH|SPFX_SEEK|SPFX_LUCK), 0,
	PHYS(20,12),	NO_DFNS,	NO_CARY,
	0,		A_LAWFUL, PM_SAMURAI, NON_PM, 4500L, 
	0,0,0),

/*
 *	Ah, never shall I forget the cry,
 *		or the shriek that shrieked he,
 *	As I gnashed my teeth, and from my sheath
 *		I drew my Snickersnee!
 *			--Koko, Lord high executioner of Titipu
 *			  (From Sir W.S. Gilbert's "The Mikado")
 */

 A("Snickersnee",		SHORT_SWORD, /*wakizashi to samurai*/
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_BEHEAD), 0, 0, /* currently wishable, or nameable by advanced tourists */
	PHYS(3,6),	NO_DFNS,	NO_CARY,						/* Tourists and Samurai can twoweapon Snickersnee */
	0, A_NONE, PM_SAMURAI, NON_PM, 1200L, 
	0,0,0),

  /*//////////////Special Monster Artifacts//////////////*/
 /*//None of these count, since they are boss treasure//*/
/*/////////////////////////////////////////////////////*/

A("Genocide", TWO_HANDED_SWORD,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_ATTK), 0, 0,
	FIRE(10,20),	NO_DFNS,	NO_CARY,
	0,	A_LAWFUL, NON_PM, NON_PM, 9999L, 
	SPFX2_FIRE|SPFX2_BLDTHRST,SPFX3_NOCNT,0),

A("Rod of Dis", MACE,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL), 0, 0,
	PHYS(10,8),	NO_DFNS,	NO_CARY,
	TAMING,	A_LAWFUL, NON_PM, NON_PM, 9999L, 
	SPFX2_RAM,SPFX3_NOCNT,0),

A("Averice", SHORT_SWORD,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL), 0, 0,
	PHYS(10,0),	NO_DFNS,	NO_CARY,
	0,	A_LAWFUL, NON_PM, NON_PM, 9999L, 
	SPFX2_STEAL,SPFX3_NOCNT,0),

A("Fire of Heaven", TRIDENT,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_ATTK), 0, 0,
	FIRE(1,0),	NO_DFNS,	NO_CARY,
	0,	A_LAWFUL, NON_PM, NON_PM, 9999L, 
	SPFX2_SILVERED|SPFX2_FIRE2|SPFX2_ELEC,SPFX3_NOCNT,0),

A("Diadem of Amnesia", DUNCE_CAP,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL), 0, 0,
	NO_ATTK,	NO_DFNS,	NO_CARY,
	CONFLICT,	A_LAWFUL, NON_PM, NON_PM, 9999L, /*was DRAIN_MEMORIES*/ 
	0,SPFX3_NOCNT,0),

A("Thunder's Voice", SILVER_DAGGER,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_ATTK), 0, 0,
	ELEC(6,6),	NO_DFNS,	NO_CARY,
	0,	A_LAWFUL, NON_PM, NON_PM, 3333L, 
	SPFX2_ELEC,SPFX3_NOCNT,0),

A("Serpent's Tooth", ATHAME,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL), 0, 0,
	NO_ATTK,	NO_DFNS,	NO_CARY,
	0,	A_LAWFUL, NON_PM, NON_PM, 3333L, 
	SPFX2_POISONED,SPFX3_NOCNT,0),

A("Unblemished Soul", UNICORN_HORN,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL), SPFX_LUCK, 0,
	NO_ATTK,	NO_DFNS,	NO_CARY,
	0,	A_LAWFUL, NON_PM, NON_PM, 3333L, 
	SPFX2_SILVERED,SPFX3_NOCNT,0),

A("Wrath of Heaven", LONG_SWORD,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_ATTK), 0, 0,
	ELEC(1,0),	NO_DFNS,	NO_CARY,
	0,	A_LAWFUL, NON_PM, NON_PM, 9999L, 
	SPFX2_SILVERED|SPFX2_FIRE|SPFX2_ELEC2,SPFX3_NOCNT,0),

A("All-seeing Eye of the Fly", HELM_OF_TELEPATHY,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL), 0, 0,
	NO_ATTK,	NO_DFNS,	NO_CARY,
	SHADOW_FLARE,	A_LAWFUL, NON_PM, NON_PM, 9999L, /* Was SLAY_LIVING */
	0,SPFX3_NOCNT,0),

A("Cold Soul", RANSEUR,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL), 0, 0,
	NO_ATTK,	NO_DFNS,	NO_CARY,
	0,	A_LAWFUL, NON_PM, NON_PM, 9999L, 
	SPFX2_ELEC|SPFX2_COLD|SPFX2_FIRE,SPFX3_NOCNT,0),

A("Sceptre of the Frozen Floor of Hell", IRON_BAR,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_ATTK), 0, 0,
	COLD(1,0),	NO_DFNS,	NO_CARY,
	BLIZAGA,	A_LAWFUL, NON_PM, NON_PM, 9999L, 
	SPFX2_COLD2,SPFX3_NOCNT,0),

A("Caress", BULLWHIP,
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_DFLAG2), 0, (M2_ELF|M2_HUMAN|M2_DWARF),
	ELEC(1,20),	ELEC(0,0),	NO_CARY,
	0, A_LAWFUL, NON_PM, NON_PM, 9999L, 
	0,SPFX3_NOCNT,0),

A("The Iconoclast", SILVER_SABER,  /*Weapon of Lixer, Prince of Hell, from Dicefreaks the Gates of Hell*/
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_DFLAG2), 0, (M2_HUMAN|M2_ELF|M2_DWARF),
	PHYS(9,99),	DFNS(AD_MAGM),	NO_CARY, /*also does +9 damage to S_ANGELs*/
	0,	A_LAWFUL, NON_PM, NON_PM, 9999L, 
	0,SPFX3_NOCNT,0),

A("Doomscreamer", TWO_HANDED_SWORD,  /*Weapon of Graz'zt, from Gord the Rogue*/
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_ATTK|SPFX_DEFN), 0, 0,
	ACID(1,0),	ACID(0,0),	NO_CARY,
	0,	A_CHAOTIC, NON_PM, NON_PM, 6660L, 
	0,SPFX3_NOCNT,0),

A("Sword of Erathaol",			LONG_SWORD,		/*  */
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_SEARCH), 0, 0,
	PHYS(7,0),	DFNS(AD_BLND),	NO_CARY,	/**/
	0, A_LAWFUL, NON_PM, NON_PM, 7500L, 	/**/
	SPFX2_SILVERED|SPFX2_BLIND|SPFX2_BRIGHT,SPFX3_NOCNT,0),

A("Saber of Sabaoth",			SILVER_SABER,		/*  */
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_SEARCH), 0, 0,
	PHYS(7,0),	DFNS(AD_BLND),	NO_CARY,	/**/
	0, A_LAWFUL, NON_PM, NON_PM, 7500L, 	/**/
	SPFX2_BLIND|SPFX2_BRIGHT,SPFX3_NOCNT,0),

A("Sword of Onoel",			TWO_HANDED_SWORD,		/*  */
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_SEARCH), 0, 0,
	PHYS(7,0),	DFNS(AD_BLND),	NO_CARY,	/**/
	0, A_LAWFUL, NON_PM, NON_PM, 7500L, 	/**/
	SPFX2_SILVERED|SPFX2_BLIND|SPFX2_BRIGHT,SPFX3_NOCNT,0),

A("Glaive of Shamsiel",			GLAIVE,		/*  */
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_SEARCH), 0, 0,
	PHYS(7,0),	DFNS(AD_BLND),	NO_CARY,	/**/
	0, A_LAWFUL, NON_PM, NON_PM, 7500L, 	/**/
	SPFX2_SILVERED|SPFX2_BLIND|SPFX2_BRIGHT,SPFX3_NOCNT,0),

A("Lance of Uriel",			LANCE,		/*  */
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_SEARCH), 0, 0,
	PHYS(7,0),	DFNS(AD_BLND),	NO_CARY,	/**/
	0, A_LAWFUL, NON_PM, NON_PM, 7500L, 	/**/
	SPFX2_SILVERED|SPFX2_BLIND|SPFX2_BRIGHT,SPFX3_NOCNT,0),

A("Hammer of Barquiel",			LUCERN_HAMMER,		/* Yes, I know it's bladed ;) */
	(SPFX_NOGEN|SPFX_RESTR|SPFX_INTEL|SPFX_SEARCH), 0, 0,
	PHYS(7,0),	DFNS(AD_BLND),	NO_CARY,	/**/
	0, A_LAWFUL, NON_PM, NON_PM, 7500L, 	/**/
	SPFX2_SILVERED|SPFX2_BLIND|SPFX2_BRIGHT,SPFX3_NOCNT,0),

/*
 *  terminator; otyp must be zero
 */
A(0, 0, 0, 0, 0, NO_ATTK, NO_DFNS, NO_CARY, 0, A_NONE, NON_PM, NON_PM, 0L, 0,0,0 )

};	/* artilist[] (or artifact_names[]) */

#undef	A

#ifndef MAKEDEFS_C
#undef	NO_ATTK
#undef	NO_DFNS
#undef	DFNS
#undef	PHYS
#undef	DRLI
#undef	COLD
#undef	FIRE
#undef	ELEC
#undef	STUN
#endif

/*artilist.h*/
