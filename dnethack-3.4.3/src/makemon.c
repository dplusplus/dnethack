/*	SCCS Id: @(#)makemon.c	3.4	2003/09/06	*/
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/* NetHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include "epri.h"
#include "emin.h"
#include "edog.h"
#ifdef REINCARNATION
#include <ctype.h>
#endif

STATIC_VAR NEARDATA struct monst zeromonst;

/* this assumes that a human quest leader or nemesis is an archetype
   of the corresponding role; that isn't so for some roles (tourist
   for instance) but is for the priests and monks we use it for... */
#define quest_mon_represents_role(mptr,role_pm) \
		(mptr->mlet == S_HUMAN && Role_if(role_pm) && \
		  (mptr->msound == MS_LEADER || mptr->msound == MS_NEMESIS))

#ifdef OVL0
STATIC_DCL boolean FDECL(uncommon, (int));
STATIC_DCL int FDECL(align_shift, (struct permonst *));
#endif /* OVL0 */
STATIC_DCL boolean FDECL(wrong_elem_type, (struct permonst *));
STATIC_DCL void FDECL(m_initgrp,(struct monst *,int,int,int));
STATIC_DCL void FDECL(m_initthrow,(struct monst *,int,int));
STATIC_DCL void FDECL(m_initweap,(struct monst *));
#ifdef OVL1
STATIC_DCL void FDECL(m_initinv,(struct monst *));
#endif /* OVL1 */

extern const int monstr[];

#define m_initsgrp(mtmp, x, y)	m_initgrp(mtmp, x, y, 3)
#define m_initlgrp(mtmp, x, y)	m_initgrp(mtmp, x, y, 10)
#define toostrong(monindx, lev) (monstr[monindx] > lev)
#define tooweak(monindx, lev)	(monstr[monindx] < lev)

#ifdef OVLB
boolean
is_home_elemental(ptr)
register struct permonst *ptr;
{
	if (ptr->mlet == S_ELEMENTAL)
	    switch (monsndx(ptr)) {
		case PM_AIR_ELEMENTAL: return Is_airlevel(&u.uz);
		case PM_FIRE_ELEMENTAL: return Is_firelevel(&u.uz);
		case PM_EARTH_ELEMENTAL: return Is_earthlevel(&u.uz);
		case PM_WATER_ELEMENTAL: return Is_waterlevel(&u.uz);
	    }
	return FALSE;
}

/*
 * Return true if the given monster cannot exist on this elemental level.
 */
STATIC_OVL boolean
wrong_elem_type(ptr)
    register struct permonst *ptr;
{
    if (ptr->mlet == S_ELEMENTAL) {
	return((boolean)(!is_home_elemental(ptr)));
    } else if (Is_earthlevel(&u.uz)) {
	/* no restrictions? */
    } else if (Is_waterlevel(&u.uz)) {
	/* just monsters that can swim */
	if(!is_swimmer(ptr)) return TRUE;
    } else if (Is_firelevel(&u.uz)) {
	if (!pm_resistance(ptr,MR_FIRE)) return TRUE;
    } else if (Is_airlevel(&u.uz)) {
	if(!(is_flyer(ptr) && ptr->mlet != S_TRAPPER) && !is_floater(ptr)
	   && !amorphous(ptr) && !noncorporeal(ptr) && !is_whirly(ptr))
	    return TRUE;
    }
    return FALSE;
}

STATIC_OVL void
m_initgrp(mtmp, x, y, n)	/* make a group just like mtmp */
register struct monst *mtmp;
register int x, y, n;
{
	coord mm;
	int mndx;
	register int cnt = rnd(n);
	struct monst *mon;
#if defined(__GNUC__) && (defined(HPUX) || defined(DGUX))
	/* There is an unresolved problem with several people finding that
	 * the game hangs eating CPU; if interrupted and restored, the level
	 * will be filled with monsters.  Of those reports giving system type,
	 * there were two DG/UX and two HP-UX, all using gcc as the compiler.
	 * hcroft@hpopb1.cern.ch, using gcc 2.6.3 on HP-UX, says that the
	 * problem went away for him and another reporter-to-newsgroup
	 * after adding this debugging code.  This has almost got to be a
	 * compiler bug, but until somebody tracks it down and gets it fixed,
	 * might as well go with the "but it went away when I tried to find
	 * it" code.
	 */
	int cnttmp,cntdiv;

	cnttmp = cnt;
# ifdef DEBUG
	pline("init group call x=%d,y=%d,n=%d,cnt=%d.", x, y, n, cnt);
# endif
	cntdiv = ((u.ulevel < 3) ? 4 : (u.ulevel < 5) ? 2 : 1);
#endif
	/* Tuning: cut down on swarming at low character levels [mrs] */
	cnt /= (u.ulevel < 3) ? 4 : (u.ulevel < 5) ? 2 : 1;
#if defined(__GNUC__) && (defined(HPUX) || defined(DGUX))
	if (cnt != (cnttmp/cntdiv)) {
		pline("cnt=%d using %d, cnttmp=%d, cntdiv=%d", cnt,
			(u.ulevel < 3) ? 4 : (u.ulevel < 5) ? 2 : 1,
			cnttmp, cntdiv);
	}
#endif
	if(!cnt) cnt++;
#if defined(__GNUC__) && (defined(HPUX) || defined(DGUX))
	if (cnt < 0) cnt = 1;
	if (cnt > 10) cnt = 10;
#endif

	mm.x = x;
	mm.y = y;
	mndx = monsndx(mtmp->data);
	if(!u.uevent.uaxus_foe || mndx > PM_QUINON || mndx < PM_MONOTON){
	 while(cnt--) {
		if (peace_minded(mtmp->data)) continue;
		/* Don't create groups of peaceful monsters since they'll get
		 * in our way.  If the monster has a percentage chance so some
		 * are peaceful and some are not, the result will just be a
		 * smaller group.
		 */
		if (enexto(&mm, mm.x, mm.y, mtmp->data)) {
		    mon = makemon(mtmp->data, mm.x, mm.y, NO_MM_FLAGS);
		    mon->mpeaceful = FALSE;
		    mon->mavenge = 0;
		    set_malign(mon);
		    /* Undo the second peace_minded() check in makemon(); if the
		     * monster turned out to be peaceful the first time we
		     * didn't create it at all; we don't want a second check.
		     */
		}
	 }
	}
	else{
	 while(cnt--) {
		if (enexto(&mm, mm.x, mm.y, mtmp->data)) {
		    mon = makemon(mtmp->data, mm.x, mm.y, NO_MM_FLAGS);
		    mon->mpeaceful = 1;
		    mon->mavenge = 0;
		    set_malign(mon);
			mtmp->mpeaceful = 0;
		}
	 }
	}
}

STATIC_OVL
void
m_initthrow(mtmp,otyp,oquan)
struct monst *mtmp;
int otyp,oquan;
{
	register struct obj *otmp;

	otmp = mksobj(otyp, TRUE, FALSE);
	otmp->quan = (long) rn1(oquan, 3);
	otmp->owt = weight(otmp);
	if (otyp == ORCISH_ARROW) otmp->opoisoned = TRUE;
	(void) mpickobj(mtmp, otmp);
}

#endif /* OVLB */
#ifdef OVL2

STATIC_OVL void
m_initweap(mtmp)
register struct monst *mtmp;
{
	register struct permonst *ptr = mtmp->data;
	register int mm = monsndx(ptr);
	struct obj *otmp;
	int chance = 0;

#ifdef REINCARNATION
	if (Is_rogue_level(&u.uz)) return;
#endif
/*
 *	first a few special cases:
 *
 *		giants get a boulder to throw sometimes.
 *		ettins get clubs
 *		kobolds get darts to throw
 *		centaurs get some sort of bow & arrows or bolts
 *		soldiers get all sorts of things.
 */
	if(mm==PM_ARGENTUM_GOLEM){
	    struct obj *otmp = mksobj(SILVER_ARROW, TRUE, FALSE);
	    otmp->blessed = FALSE;
	    otmp->cursed = FALSE;
		otmp->quan += 43;
		(void) mpickobj(mtmp,otmp);
		if(!rn2(6))mongets(mtmp,SILVER_SABER);
		else mongets(mtmp,SILVER_DAGGER);
		return;
	}
	if(mm==PM_IRON_GOLEM){
		    if(rn2(20))(void) mongets(mtmp, GLAIVE);
			else (void) mongets(mtmp, LANCE);
			(void) mongets(mtmp, WAR_HAMMER);
			return;
	}

/*	if(mm==PM_SERVANT_OF_THE_UNKNOWN_GOD){
		mongets(mtmp, CLUB);
		return;
	}
*/
	switch (ptr->mlet) {
	    case S_GIANT:
		if (rn2(2)) (void)mongets(mtmp, (mm != PM_ETTIN) ?
				    BOULDER : CLUB);
		break;
		case S_IMP:{
		    int w1 = 0, w2 = 0;
			switch (mm){
			case PM_LEGION_DEVIL_GRUNT:
			      w1 = rn1(BEC_DE_CORBIN - PARTISAN + 1, PARTISAN);
			      w2 = rn2(2) ? DAGGER : KNIFE;
				  (void)mongets(mtmp, BOW);
				  m_initthrow(mtmp, ARROW, 5);
				  if(rnd(20)<3)(void)mongets(mtmp, rnd(WAN_LIGHTNING-WAN_LIGHT)+WAN_LIGHT);
			break;
			case PM_LEGION_DEVIL_SOLDIER:
			      w1 = rn1(BEC_DE_CORBIN - PARTISAN + 1, PARTISAN);
			      w2 = rn2(2) ? DAGGER : KNIFE;
				  (void)mongets(mtmp, BOW);
				  m_initthrow(mtmp, ARROW, 10);
				  if(rnd(10)<3)(void)mongets(mtmp, rnd(WAN_LIGHTNING-WAN_LIGHT)+WAN_LIGHT);
			break;
			case PM_LEGION_DEVIL_SERGEANT:
			      w1 = rn1(BEC_DE_CORBIN - PARTISAN + 1, PARTISAN);
			      w2 = rn2(2) ? DAGGER : KNIFE;
				  (void)mongets(mtmp, BOW);
				  m_initthrow(mtmp, ARROW, 20);
				  (void)mongets(mtmp, rnd(WAN_LIGHTNING-WAN_LIGHT)+WAN_LIGHT);
			break;
			case PM_LEGION_DEVIL_CAPTAIN:
			      w1 = rn1(BEC_DE_CORBIN - PARTISAN + 1, PARTISAN);
			      w2 = rn2(2) ? DAGGER : KNIFE;
				  (void)mongets(mtmp, BOW);
				  m_initthrow(mtmp, ARROW, 30);
				  (void)mongets(mtmp, rnd(WAN_LIGHTNING-WAN_LIGHT)+WAN_LIGHT);
				  (void)mongets(mtmp, rnd(WAN_LIGHTNING-WAN_LIGHT)+WAN_LIGHT);
			break;
			case PM_GLASYA:
				otmp = mksobj(BULLWHIP, TRUE, FALSE);
				otmp = oname(otmp, artiname(ART_CARESS));
				otmp->spe = 9;
				otmp->blessed = FALSE;
				otmp->cursed = FALSE;
				(void) mpickobj(mtmp,otmp);
			break;
			}
		    if (w1) (void)mongets(mtmp, w1);
		    if (w2) (void)mongets(mtmp, w2);
		break;}
	    case S_HUMAN:
			if(mm == PM_ELDER_PRIEST){
				otmp = mksobj(QUARTERSTAFF, TRUE, FALSE);
				bless(otmp);
				otmp->spe = 3;
				(void) mpickobj(mtmp, otmp);
				otmp = mksobj(CLOAK_OF_MAGIC_RESISTANCE, TRUE, FALSE);
				bless(otmp);
				otmp->spe = 3;
				(void) mpickobj(mtmp, otmp);
				otmp = mksobj(POT_FULL_HEALING, TRUE, FALSE);
				bless(otmp);
				(void) mpickobj(mtmp, otmp);
				otmp = mksobj(POT_EXTRA_HEALING, TRUE, FALSE);
				bless(otmp);
				otmp->quan = 3;
				(void) mpickobj(mtmp, otmp);
				return;//no random stuff!
			}
		if(mm == PM_DAEMON){
			otmp = mksobj(SILVER_SABER, TRUE, FALSE);
			otmp = oname(otmp, artiname(ART_ICONOCLAST));
			otmp->spe = 9;
			otmp->blessed = FALSE;
			otmp->cursed = FALSE;
			(void) mpickobj(mtmp,otmp);

			(void)mongets(mtmp, ELVEN_MITHRIL_COAT);
			(void)mongets(mtmp, ORCISH_SHIELD);
			(void)mongets(mtmp, CLOAK_OF_MAGIC_RESISTANCE);
		    (void)mongets(mtmp, ELVEN_LEATHER_HELM);
		    (void)mongets(mtmp, ELVEN_BOOTS);
		    (void)mongets(mtmp, ELVEN_DAGGER);
		    (void)mongets(mtmp, ORCISH_DAGGER);
		    (void)mongets(mtmp, DAGGER);
		    (void)mongets(mtmp, STILETTO);

			otmp = mksobj(POT_FULL_HEALING, TRUE, FALSE);
			bless(otmp);
			(void) mpickobj(mtmp, otmp);
			otmp = mksobj(POT_EXTRA_HEALING, TRUE, FALSE);
			bless(otmp);
			otmp->quan = 3;
			(void) mpickobj(mtmp, otmp);
		}
		else if(is_mercenary(ptr)) {
		    int w1 = 0, w2 = 0;
		    switch (mm) {
			case PM_WATCHMAN:
			case PM_SOLDIER:
			  if (!rn2(3)) {
			      w1 = rn1(BEC_DE_CORBIN - PARTISAN + 1, PARTISAN);
			      w2 = rn2(2) ? DAGGER : KNIFE;
			  } else w1 = rn2(2) ? SPEAR : SHORT_SWORD;
			  break;
			case PM_SERGEANT:
			  w1 = rn2(2) ? FLAIL : MACE;
			  break;
			case PM_LIEUTENANT:
			  w1 = rn2(2) ? BROADSWORD : LONG_SWORD;
			  break;
			case PM_CAPTAIN:
			case PM_WATCH_CAPTAIN:
			  w1 = rn2(2) ? LONG_SWORD : SILVER_SABER;
			  break;
			default:
			  if (!rn2(4)) w1 = DAGGER;
			  if (!rn2(7)) w2 = SPEAR;
			  break;
		    }
		    if (w1) (void)mongets(mtmp, w1);
		    if (!w2 && w1 != DAGGER && !rn2(4)) w2 = KNIFE;
		    if (w2) (void)mongets(mtmp, w2);
		} else if (is_elf(ptr)) {
		    if (rn2(2))
			(void) mongets(mtmp,
				   rn2(2) ? ELVEN_MITHRIL_COAT : ELVEN_CLOAK);
		    if (rn2(2)) (void)mongets(mtmp, ELVEN_LEATHER_HELM);
		    else if (!rn2(4)) (void)mongets(mtmp, ELVEN_BOOTS);
		    if (rn2(2)) (void)mongets(mtmp, ELVEN_DAGGER);
		    switch (rn2(3)) {
			case 0:
			    if (!rn2(4)) (void)mongets(mtmp, ELVEN_SHIELD);
			    if (rn2(3)) (void)mongets(mtmp, ELVEN_SHORT_SWORD);
			    (void)mongets(mtmp, ELVEN_BOW);
			    m_initthrow(mtmp, ELVEN_ARROW, 12);
			    break;
			case 1:
			    (void)mongets(mtmp, ELVEN_BROADSWORD);
			    if (rn2(2)) (void)mongets(mtmp, ELVEN_SHIELD);
			    break;
			case 2:
			    if (rn2(2)) {
				(void)mongets(mtmp, ELVEN_SPEAR);
				(void)mongets(mtmp, ELVEN_SHIELD);
			    }
			    break;
		    }
		    if (mm == PM_ELVENKING) {
			if (rn2(3) || (in_mklev && Is_earthlevel(&u.uz)))
			    (void)mongets(mtmp, PICK_AXE);
			if (!rn2(50)) (void)mongets(mtmp, CRYSTAL_BALL);
		    }
		} else if (ptr->msound == MS_PRIEST ||
			quest_mon_represents_role(ptr,PM_PRIEST)) {
		    otmp = mksobj(MACE, FALSE, FALSE);
		    if(otmp) {
			otmp->spe = rnd(3);
			if(!rn2(2)) curse(otmp);
			(void) mpickobj(mtmp, otmp);
		    }
		}
		else{
			switch(mm){
			case PM_SIR_GARLAND:
				(void) mongets(mtmp, LONG_SWORD);
				(void) mongets(mtmp, LANCE);
				(void) mongets(mtmp, PLATE_MAIL);
			break;
			case PM_GARLAND:
				(void) mongets(mtmp, RUNESWORD);
				(void) mongets(mtmp, PLATE_MAIL);
			break;
			case PM_NINJA: 
				(void)mongets(mtmp, BROADSWORD);
				chance = d(1,100);
				if(chance > 95) (void)mongets(mtmp, SHURIKEN);
				else if(chance > 75) (void)mongets(mtmp, DART);
				else{ 
					(void) mongets(mtmp, DAGGER);
					(void) mongets(mtmp, DAGGER);
				}
			break;
			case PM_NURSE:
				(void) mongets(mtmp, SCALPEL);
			return;
			break;
			case PM_MAID: //ninja maids
				chance = d(1,100);
				if(chance > 75) (void)mongets(mtmp, SHURIKEN);
				else if(chance > 50) (void)mongets(mtmp, DART);
				if(chance > 99) (void)mongets(mtmp, SILVER_DAGGER);
				(void) mongets(mtmp, DAGGER);
				(void) mongets(mtmp, DAGGER);
				(void) mongets(mtmp, DAGGER);
				(void) mongets(mtmp, DAGGER);
				chance = d(1,100);
				if(Role_if(PM_SAMURAI))
					if(chance > 95) (void)mongets(mtmp, GLAIVE);
					else if(chance > 75) (void)mongets(mtmp, KATANA);
					else if(chance > 50) (void)mongets(mtmp, BROADSWORD);
					else (void)mongets(mtmp, STILETTO);
				else if(chance > 70) (void)mongets(mtmp, KATANA);
				else (void)mongets(mtmp, STILETTO);
			break;
			}
		}
		break;

	    case S_ANGEL:
		{
/*			if(ptr == &mons[PM_DESTROYER]){
				struct obj *otmp = mksobj(BROADSWORD, TRUE, FALSE);
				otmp->blessed = FALSE;
				otmp->cursed = TRUE;
				otmp->spe = 10;
				(void) mpickobj(mtmp,otmp);

				otmp = mksobj(BROADSWORD, TRUE, FALSE);
				otmp->blessed = FALSE;
				otmp->cursed = TRUE;
				otmp->spe = 10;
				(void) mpickobj(mtmp,otmp);

				otmp = mksobj(RIN_CONFLICT, TRUE, FALSE);
				otmp->blessed = FALSE;
				otmp->cursed = TRUE;
				(void) mpickobj(mtmp,otmp);

				otmp = mksobj(RIN_INCREASE_DAMAGE, TRUE, FALSE);
				otmp->blessed = FALSE;
				otmp->cursed = TRUE;
				otmp->spe = 19;
				(void) mpickobj(mtmp,otmp);

				otmp = mksobj(SKELETON_KEY, TRUE, FALSE);
				otmp = oname(otmp, artiname(ART_THIRD_KEY_OF_LAW));
				otmp->blessed = FALSE;
				otmp->cursed = FALSE;
				(void) mpickobj(mtmp,otmp);
				return;

			}
			else if(ptr == &mons[PM_DANCER]){
				struct obj *otmp = mksobj(STILETTO, TRUE, FALSE);
				otmp->blessed = FALSE;
				otmp->cursed = TRUE;
				otmp->spe = 19;
				(void) mpickobj(mtmp,otmp);

				otmp = mksobj(STILETTO, TRUE, FALSE);
				otmp->blessed = FALSE;
				otmp->cursed = TRUE;
				otmp->spe = 19;
				(void) mpickobj(mtmp,otmp);

				otmp = mksobj(RIN_INCREASE_DAMAGE, TRUE, FALSE);
				otmp->blessed = TRUE;
				otmp->cursed = FALSE;
				otmp->spe = 28;
				(void) mpickobj(mtmp,otmp);
				return;

			}
*/
			/*else*/ {
				int spe2;
	
			    /* create minion stuff; can't use mongets */
			    otmp = mksobj(LONG_SWORD, FALSE, FALSE);
	
			    /* maybe make it special */
			    if (!rn2(20) || is_lord(ptr))
				otmp = oname(otmp, artiname(
					rn2(2) ? ART_DEMONBANE : ART_SUNSWORD));
			    bless(otmp);
			    otmp->oerodeproof = TRUE;
			    spe2 = rn2(4);
			    otmp->spe = max(otmp->spe, spe2);
			    (void) mpickobj(mtmp, otmp);
	
			    otmp = mksobj(!rn2(4) || is_lord(ptr) ?
					  SHIELD_OF_REFLECTION : LARGE_SHIELD,
					  FALSE, FALSE);
			    otmp->cursed = FALSE;
			    otmp->oerodeproof = TRUE;
			    otmp->spe = 0;
			    (void) mpickobj(mtmp, otmp);
			}
		}
		break;
		
		case S_GNOME:
	      {
		int bias;

		bias = is_lord(ptr) + is_prince(ptr) * 2 + extra_nasty(ptr);
		switch(rnd(14 - (2 * bias))) {
		    case 1:
				m_initthrow(mtmp, DART, 12);
			break;
		    case 2:
			    (void) mongets(mtmp, CROSSBOW);
			    m_initthrow(mtmp, CROSSBOW_BOLT, 12);
			break;
		    case 3:
			    (void) mongets(mtmp, SLING);
			    m_initthrow(mtmp, ROCK, 12);
				(void) mongets(mtmp, AKLYS);
			break;
		    case 4:
				m_initthrow(mtmp, DAGGER, 3);
			break;
		    case 5:
				(void) mongets(mtmp, AKLYS);
			    (void) mongets(mtmp, SLING);
			    m_initthrow(mtmp, ROCK, 3);
			break;
		    default:
				(void) mongets(mtmp, AKLYS);
			break;
		}
		if (In_mines(&u.uz)) {//note, gnomish wizards never have pointy hats.
			/* cm: Gnomes in dark mines have candles lit. */
			    otmp = mksobj(GNOMISH_POINTY_HAT, TRUE, FALSE);
			    (void) mpickobj(mtmp, otmp);
				if (!levl[mtmp->mx][mtmp->my].lit) {
					begin_burn(otmp, FALSE);
			    }	
		}
		else//Outside the mines, only one in 6 gnomes have hats.
		    if(!rn2(6)) (void)mongets(mtmp, GNOMISH_POINTY_HAT);
		  } //end case brackets

		break;

		case S_HUMANOID:
		if (mm == PM_HOBBIT) {
		    switch (rn2(3)) {
			case 0:
			    (void)mongets(mtmp, DAGGER);
			    break;
			case 1:
			    (void)mongets(mtmp, ELVEN_DAGGER);
			    break;
			case 2:
			    (void)mongets(mtmp, SLING);
			    break;
		      }
		    if (!rn2(10)) (void)mongets(mtmp, ELVEN_MITHRIL_COAT);
		    if (!rn2(10)) (void)mongets(mtmp, DWARVISH_CLOAK);
		} else if (is_dwarf(ptr)) { //slightly rearanged code so more dwarves get helms -D_E
		    if (rn2(7)) (void)mongets(mtmp, DWARVISH_CLOAK);
		    if (rn2(7)) (void)mongets(mtmp, IRON_SHOES);
			if (!rn2(4)) {
				(void)mongets(mtmp, DWARVISH_SHORT_SWORD);
			} else {
				(void)mongets(mtmp, DAGGER);
			}
			/* note: you can't use a mattock with a shield */
			if (!rn2(4)) (void)mongets(mtmp, DWARVISH_MATTOCK);
			else {
				(void)mongets(mtmp, !rn2(3) ? PICK_AXE : AXE);
				if(!rn2(3)) (void)mongets(mtmp, DWARVISH_ROUNDSHIELD);
			}
			if (In_mines(&u.uz)) {
			/* MRKR: Dwarves in dark mines have their lamps on. */
			    otmp = mksobj(DWARVISH_IRON_HELM, TRUE, FALSE);
			    (void) mpickobj(mtmp, otmp);
				    if (!levl[mtmp->mx][mtmp->my].lit) {
					begin_burn(otmp, FALSE);
			    }	
			}
			else
			    (void)mongets(mtmp, DWARVISH_IRON_HELM);
 			if (!rn2(3))
			    (void)mongets(mtmp, DWARVISH_MITHRIL_COAT);
		}
		break;
	    case S_KETER:
			if (mm == PM_CHOKHMAH_SEPHIRAH){
				otmp = mksobj(LONG_SWORD,
						  TRUE, rn2(13) ? FALSE : TRUE);
				otmp->spe = 7;
				otmp->oerodeproof = 1;
				(void) mpickobj(mtmp, otmp);
			}
		break;
	    case S_ORC:
		if(rn2(2)) (void)mongets(mtmp, ORCISH_HELM);
		switch (mm != PM_ORC_CAPTAIN ? mm :
			rn2(2) ? PM_MORDOR_ORC : PM_URUK_HAI) {
		    case PM_MORDOR_ORC:
			if(!rn2(3)) (void)mongets(mtmp, SCIMITAR);
			if(!rn2(3)) (void)mongets(mtmp, ORCISH_SHIELD);
			if(!rn2(3)) (void)mongets(mtmp, KNIFE);
			if(!rn2(3)) (void)mongets(mtmp, ORCISH_CHAIN_MAIL);
			break;
		    case PM_URUK_HAI:
			if(!rn2(3)) (void)mongets(mtmp, ORCISH_CLOAK);
			if(!rn2(3)) (void)mongets(mtmp, ORCISH_SHORT_SWORD);
			if(!rn2(3)) (void)mongets(mtmp, IRON_SHOES);
			if(!rn2(3)) {
			    (void)mongets(mtmp, ORCISH_BOW);
			    m_initthrow(mtmp, ORCISH_ARROW, 12);
			}
			if(!rn2(3)) (void)mongets(mtmp, URUK_HAI_SHIELD);
			break;
		    default:
			if (mm != PM_ORC_SHAMAN && rn2(2))
			  (void)mongets(mtmp, (mm == PM_GOBLIN || rn2(2) == 0)
						   ? ORCISH_DAGGER : SCIMITAR);
		}
		break;
	    case S_OGRE:
		if (!rn2(mm == PM_OGRE_KING ? 3 : mm == PM_OGRE_LORD ? 6 : 12))
		    (void) mongets(mtmp, BATTLE_AXE);
		else
		    (void) mongets(mtmp, CLUB);
		if(mm == PM_SIEGE_OGRE){
		    struct obj *otmp = mksobj(ARROW, TRUE, FALSE);
		    otmp->blessed = FALSE;
		    otmp->cursed = FALSE;
			otmp->quan = 240;
			(void) mpickobj(mtmp,otmp);
		}
		break;
	    case S_TROLL:
		if (!rn2(2)) switch (rn2(4)) {
		    case 0: (void)mongets(mtmp, RANSEUR); break;
		    case 1: (void)mongets(mtmp, PARTISAN); break;
		    case 2: (void)mongets(mtmp, GLAIVE); break;
		    case 3: (void)mongets(mtmp, SPETUM); break;
		}
		break;
	    case S_KOBOLD:
		if (!rn2(4)) m_initthrow(mtmp, DART, 12);
		break;

	    case S_CENTAUR:
		if (rn2(2)) {
		    if(ptr == &mons[PM_FOREST_CENTAUR]) {
			(void)mongets(mtmp, BOW);
			m_initthrow(mtmp, ARROW, 12);
		    } else {
			(void)mongets(mtmp, CROSSBOW);
			m_initthrow(mtmp, CROSSBOW_BOLT, 12);
		    }
		}
		break;
	    case S_WRAITH:
		(void)mongets(mtmp, KNIFE);
		(void)mongets(mtmp, LONG_SWORD);
		break;
	    case S_ZOMBIE:
		if (!rn2(4)) (void)mongets(mtmp, LEATHER_ARMOR);
		if (!rn2(4))
			(void)mongets(mtmp, (rn2(3) ? KNIFE : SHORT_SWORD));
		break;
	    case S_LIZARD:
		if (mm == PM_SALAMANDER)
			(void)mongets(mtmp, (rn2(7) ? SPEAR : rn2(3) ?
					     TRIDENT : STILETTO));
		break;
	    case S_DEMON:

		if(mm>PM_NESSIAN_PIT_FIEND) return; //Lords handled above, no random cursed stuff!
		switch (mm) {
		    case PM_BALROG:
				(void)mongets(mtmp, BULLWHIP);
				(void)mongets(mtmp, BROADSWORD);
				return; //bypass general weapons
			break;
/*		    case PM_PLAGUE_DAEMON:
				(void)mongets(mtmp, SCIMITAR);
				(void)mongets(mtmp, BOW);
				m_initthrow(mtmp, ARROW, 20);
				return;//bypass general weapons
			break;
*/		    case PM_HORNED_DEVIL:
				(void)mongets(mtmp, rn2(4) ? TRIDENT : BULLWHIP);
			break;
/*		    case PM_ORCUS:
			(void)mongets(mtmp, WAN_DEATH); // the Wand of Orcus
			break;
		    case PM_HORNED_DEVIL:
			(void)mongets(mtmp, rn2(4) ? TRIDENT : BULLWHIP);
			break;
			case PM_BAEL:
			(void) mongets(mtmp, TWO_HANDED_SWORD);
			(void) mongets(mtmp, GAUNTLETS_OF_POWER);
			break;
		    case PM_DISPATER:
//			(void)mongets(mtmp, WAN_STRIKING);
			(void) mongets(mtmp, MACE);
			(void) mongets(mtmp, SPE_CHARM_MONSTER);
			(void) mongets(mtmp, SCR_TAMING);
			(void) mongets(mtmp, SCR_TAMING);
			break;
		    case PM_YEENOGHU:
			(void)mongets(mtmp, FLAIL);
			break;
			case PM_BAPHOMET:
				(void)mongets(mtmp, BATTLE_AXE);
				(void)mongets(mtmp, SPE_MAGIC_MAPPING);
				(void)mongets(mtmp, SCR_MAGIC_MAPPING);
				(void)mongets(mtmp, SCR_MAGIC_MAPPING);
			break;
			case PM_CHILD_LILITH:
				(void)mongets(mtmp, OILSKIN_CLOAK);
				(void)mongets(mtmp, CRYSKNIFE);
			break;
			case PM_MOTHER_LILITH:
				(void)mongets(mtmp, CLOAK_OF_MAGIC_RESISTANCE);
				(void)mongets(mtmp, ATHAME);
			break;
			case PM_CRONE_LILITH:
			(void)mongets(mtmp, BOULDER);
			(void)mongets(mtmp, QUARTERSTAFF);
			(void)mongets(mtmp, UNICORN_HORN);
			(void)mongets(mtmp, CRYSTAL_BALL);
			break;
*/		}
		/* prevent djinnis and mail daemons from leaving objects when
		 * they vanish
		 */
		if (!is_demon(ptr)) break;
		/* fall thru */
/*
 *	Now the general case, Some chance of getting some type
 *	of weapon for "normal" monsters.  Certain special types
 *	of monsters will get a bonus chance or different selections.
 */
	    default:
	      {
		int bias;

		bias = is_lord(ptr) + is_prince(ptr) * 2 + extra_nasty(ptr);
		switch(rnd(14 - (2 * bias))) {
		    case 1:
			if(strongmonst(ptr)) (void) mongets(mtmp, BATTLE_AXE);
			else m_initthrow(mtmp, DART, 12);
			break;
		    case 2:
			if(strongmonst(ptr))
			    (void) mongets(mtmp, TWO_HANDED_SWORD);
			else {
			    (void) mongets(mtmp, CROSSBOW);
			    m_initthrow(mtmp, CROSSBOW_BOLT, 12);
			}
			break;
		    case 3:
			(void) mongets(mtmp, BOW);
			m_initthrow(mtmp, ARROW, 12);
			break;
		    case 4:
			if(strongmonst(ptr)) (void) mongets(mtmp, LONG_SWORD);
			else m_initthrow(mtmp, DAGGER, 3);
			break;
		    case 5:
			if(strongmonst(ptr))
			    (void) mongets(mtmp, LUCERN_HAMMER);
			else (void) mongets(mtmp, AKLYS);
			break;
		    default:
			break;
		}
	      }
	      break;
	}
	if ((int) mtmp->m_lev > rn2(75))
		(void) mongets(mtmp, rnd_offensive_item(mtmp));
}

#endif /* OVL2 */
#ifdef OVL1

#ifdef GOLDOBJ
/*
 *   Makes up money for monster's inventory.
 *   This will change with silver & copper coins
 */
void 
mkmonmoney(mtmp, amount)
struct monst *mtmp;
long amount;
{
    struct obj *gold = mksobj(GOLD_PIECE, FALSE, FALSE);
    gold->quan = amount;
    add_to_minv(mtmp, gold);
}
#endif

STATIC_OVL void
m_initinv(mtmp)
register struct	monst	*mtmp;
{
	register int cnt;
	register struct obj *otmp;
	register struct permonst *ptr = mtmp->data;
#ifdef REINCARNATION
	if (Is_rogue_level(&u.uz)) return;
#endif
/*
 *	Soldiers get armour & rations - armour approximates their ac.
 *	Nymphs may get mirror or potion of object detection.
 */
	switch(ptr->mlet) {

	    case S_HUMAN:
		if(is_mercenary(ptr)) {
		    register int mac;
			
		    switch(monsndx(ptr)) {
			case PM_GUARD: mac = -1; break;
			case PM_SOLDIER: mac = 3; break;
			case PM_SERGEANT: mac = 0; break;
			case PM_LIEUTENANT: mac = -2; break;
			case PM_CAPTAIN: mac = -3; break;
			case PM_WATCHMAN: mac = 3; break;
			case PM_WATCH_CAPTAIN: mac = -2; break;
			default: impossible("odd mercenary %d?", monsndx(ptr));
				mac = 0;
				break;
		    }

		    if (mac < -1 && rn2(5))
			mac += 7 + mongets(mtmp, PLATE_MAIL);
		    else if (mac < 3 && rn2(5))
			mac += 6 + mongets(mtmp, (rn2(3)) ?
					   SPLINT_MAIL : BANDED_MAIL);
		    else if (rn2(5))
			mac += 3 + mongets(mtmp, (rn2(3)) ?
					   RING_MAIL : STUDDED_LEATHER_ARMOR);
		    else
			mac += 2 + mongets(mtmp, LEATHER_ARMOR);

		    if (mac < 10 && rn2(3))
			mac += 1 + mongets(mtmp, HELMET);
		    else if (mac < 10 && rn2(2))
			mac += 1 + mongets(mtmp, FLACK_HELMET);
		    if (mac < 10 && rn2(3))
			mac += 1 + mongets(mtmp, SMALL_SHIELD);
		    else if (mac < 10 && rn2(2))
			mac += 2 + mongets(mtmp, LARGE_SHIELD);
		    if (mac < 10 && rn2(3))
			mac += 1 + mongets(mtmp, LOW_BOOTS);
		    else if (mac < 10 && rn2(2))
			mac += 2 + mongets(mtmp, HIGH_BOOTS);
		    if (mac < 10 && rn2(3))
			mac += 1 + mongets(mtmp, LEATHER_GLOVES);
		    else if (mac < 10 && rn2(2))
			mac += 1 + mongets(mtmp, LEATHER_CLOAK);

		    if(ptr != &mons[PM_GUARD] &&
			ptr != &mons[PM_WATCHMAN] &&
			ptr != &mons[PM_WATCH_CAPTAIN]) {
			if (!rn2(3)) (void) mongets(mtmp, K_RATION);
			if (!rn2(2)) (void) mongets(mtmp, C_RATION);
			if (ptr != &mons[PM_SOLDIER] && !rn2(3))
				(void) mongets(mtmp, BUGLE);
		    } else
			   if (ptr == &mons[PM_WATCHMAN] && rn2(3))
				(void) mongets(mtmp, TIN_WHISTLE);
		} else if (ptr == &mons[PM_SHOPKEEPER]) {
		    (void) mongets(mtmp,SKELETON_KEY);
		    switch (rn2(4)) {
		    /* MAJOR fall through ... */
		    case 0: (void) mongets(mtmp, WAN_MAGIC_MISSILE);
		    case 1: (void) mongets(mtmp, POT_EXTRA_HEALING);
		    case 2: (void) mongets(mtmp, POT_HEALING);
				break;
		    case 3: (void) mongets(mtmp, WAN_STRIKING);
		    }
		} else if (ptr->msound == MS_PRIEST ||
			quest_mon_represents_role(ptr,PM_PRIEST)) {
		    (void) mongets(mtmp, rn2(7) ? ROBE :
					     rn2(3) ? CLOAK_OF_PROTECTION :
						 CLOAK_OF_MAGIC_RESISTANCE);
		    (void) mongets(mtmp, SMALL_SHIELD);
#ifndef GOLDOBJ
		    mtmp->mgold = (long)rn1(10,20);
#else
		    mkmonmoney(mtmp,(long)rn1(10,20));
#endif
		} else if (quest_mon_represents_role(ptr,PM_MONK)) {
		    (void) mongets(mtmp, rn2(11) ? ROBE :
					     CLOAK_OF_MAGIC_RESISTANCE);
		} else if(ptr == &mons[PM_MEDUSA]){
		 struct engr *oep = engr_at(mtmp->mx,mtmp->my);
		 int i;
		 for(i=0; i<3; i++){
			if(!oep){
				make_engr_at(mtmp->mx, mtmp->my,
			     "", 0L, DUST);
				oep = engr_at(mtmp->mx,mtmp->my);
			}
			oep->ward_id = GORGONEION;
			oep->halu_ward = 0;
			oep->ward_type = BURN;
			oep->complete_wards = 1;
			rloc_engr(oep);
		 }
		}
		break;
//		case S_IMP:
//			switch(monsndx(ptr)) {
//			case 
//			}
//		break;
	    case S_NYMPH:
		if(!rn2(2)) (void) mongets(mtmp, MIRROR);
		if(!rn2(2)) (void) mongets(mtmp, POT_OBJECT_DETECTION);
		break;
		case S_FUNGUS:
		{
			int i = 0;
			int n = 0;
			int id = 0;
			int rng = 0;
			switch(monsndx(ptr)){
				case PM_MIGO_WORKER:
					rng = LAST_GEM-DILITHIUM_CRYSTAL;
					n = rn2(8);
					for(i=0; i<n; i++) (void)mongets(mtmp, rn2(rng)+DILITHIUM_CRYSTAL);
					n = rn2(10);
					for(i=0; i<n; i++) (void)mongets(mtmp, FLINT);
					n = rn2(20);
					for(i=0; i<n; i++) (void)mongets(mtmp, ROCK);
				break;
				case PM_MIGO_SOLDIER:
					if(!rn2(4)){
						otmp = mksobj(WAN_LIGHTNING, TRUE, FALSE);
						bless(otmp);
						otmp->recharged=7;
						otmp->spe = 2;
						(void) mpickobj(mtmp, otmp);
					}
					n = rn2(200)+200;
					for(i=0; i<n; i++) (void)mongets(mtmp, GOLD_PIECE);
				break;
				case PM_MIGO_PHILOSOPHER:
					if(!rn2(6)){ 
						otmp = mksobj(WAN_LIGHTNING, TRUE, FALSE);
						bless(otmp);
						otmp->recharged=7;
						otmp->spe = 3;
						(void) mpickobj(mtmp, otmp);
					}
					rng = LAST_GEM-DILITHIUM_CRYSTAL;
					n = rn2(3);
					for(i=0; i<n; i++) (void)mongets(mtmp, rn2(rng)+DILITHIUM_CRYSTAL);
					rng = SCR_STINKING_CLOUD-SCR_ENCHANT_ARMOR;
					n = rn2(4)+3;
					for(i=0; i<n; i++) (void)mongets(mtmp, rn2(rng)+SCR_ENCHANT_ARMOR);
				break;
				case PM_MIGO_QUEEN:
					
					otmp = mksobj(WAN_LIGHTNING, TRUE, FALSE);
					bless(otmp);
					otmp->recharged=2;
					otmp->spe = 6;
					(void) mpickobj(mtmp, otmp);
					
					rng = LAST_GEM-DILITHIUM_CRYSTAL;
					n = rn2(10);
					for(i=0; i<n; i++) (void)mongets(mtmp, rn2(rng)+DILITHIUM_CRYSTAL);
					rng = SCR_STINKING_CLOUD-SCR_ENCHANT_ARMOR;
					n = rn2(4);
					for(i=0; i<n; i++) (void)mongets(mtmp, rn2(rng)+SCR_ENCHANT_ARMOR);
				break;
			}
		}
		break;
	    case S_GIANT:
		if (ptr == &mons[PM_MINOTAUR]) {
		    if (!rn2(3) || (in_mklev && Is_earthlevel(&u.uz)))
			(void) mongets(mtmp, WAN_DIGGING);
		} else if (is_giant(ptr)) {
		    for (cnt = rn2((int)(mtmp->m_lev / 2)); cnt; cnt--) {
			otmp = mksobj(rnd_class(DILITHIUM_CRYSTAL,LUCKSTONE-1),
				      FALSE, FALSE);
			otmp->quan = (long) rn1(2, 3);
			otmp->owt = weight(otmp);
			(void) mpickobj(mtmp, otmp);
		    }
		}
		break;
	    case S_WRAITH:
		if (ptr == &mons[PM_NAZGUL]) {
			otmp = mksobj(RIN_INVISIBILITY, FALSE, FALSE);
			curse(otmp);
			(void) mpickobj(mtmp, otmp);
		}
		break;
	    case S_LICH:
		if (ptr == &mons[PM_DEATH_KNIGHT]){
			(void)mongets(mtmp, RUNESWORD);
		}
		if (ptr == &mons[PM_MASTER_LICH] && !rn2(13))
			(void)mongets(mtmp, (rn2(7) ? ATHAME : WAN_NOTHING));
		else if (ptr == &mons[PM_ARCH_LICH] && !rn2(3)) {
			otmp = mksobj(rn2(3) ? ATHAME : QUARTERSTAFF,
				      TRUE, rn2(13) ? FALSE : TRUE);
			if (otmp->spe < 2) otmp->spe = rnd(3);
			if (!rn2(4)) otmp->oerodeproof = 1;
			(void) mpickobj(mtmp, otmp);
		} else if(ptr == &mons[PM_ALHOON]){
				struct obj *otmp = mksobj(SKELETON_KEY, TRUE, FALSE);
				otmp = oname(otmp, artiname(ART_SECOND_KEY_OF_NEUTRALITY));
				if(!otmp->oartifact) otmp = oname(otmp, artiname(ART_THIRD_KEY_OF_NEUTRALITY));
				otmp->blessed = FALSE;
				otmp->cursed = FALSE;
				(void) mpickobj(mtmp,otmp);
			}

		break;
	    case S_MUMMY:
		if (rn2(7)) (void)mongets(mtmp, MUMMY_WRAPPING);
		break;
	    case S_QUANTMECH:
		if (!rn2(20)) {
			otmp = mksobj(LARGE_BOX, FALSE, FALSE);
			otmp->spe = 1; /* flag for special box */
			otmp->owt = weight(otmp);
			(void) mpickobj(mtmp, otmp);
		}
		break;
	    case S_LEPRECHAUN:
#ifndef GOLDOBJ
		mtmp->mgold = (long) d(level_difficulty(), 30);
#else
		mkmonmoney(mtmp, (long) d(level_difficulty(), 30));
#endif
		break;
	    case S_DEMON:
	    	/* moved here from m_initweap() because these don't
		   have AT_WEAP so m_initweap() is not called for them */
			switch(monsndx(ptr)){
			case PM_ICE_DEVIL:
				  if(!rn2(4)) (void)mongets(mtmp, TRIDENT);
				  else (void)mongets(mtmp, SPEAR);
			break;
///////////////////////////////
			case PM_YEENOGHU:
				(void)mongets(mtmp, FLAIL);
				(void)mongets(mtmp, GAUNTLETS_OF_DEXTERITY);
				(void) mongets(mtmp, POT_EXTRA_HEALING);
				(void) mongets(mtmp, POT_EXTRA_HEALING);
			break;
			case PM_BAPHOMET:
				(void)mongets(mtmp, BATTLE_AXE);
				(void)mongets(mtmp, SPE_MAGIC_MAPPING);
				(void)mongets(mtmp, SCR_MAGIC_MAPPING);
				(void)mongets(mtmp, SCR_MAGIC_MAPPING);
				(void) mongets(mtmp, POT_EXTRA_HEALING);
			break;
			case PM_JUIBLEX:
				(void) mongets(mtmp, POT_EXTRA_HEALING);
				(void) mongets(mtmp, POT_EXTRA_HEALING);
				(void) mongets(mtmp, POT_EXTRA_HEALING);
			break;
			case PM_ZUGGTMOY:
				(void) mongets(mtmp, POT_EXTRA_HEALING);
				(void) mongets(mtmp, POT_EXTRA_HEALING);
				(void) mongets(mtmp, POT_EXTRA_HEALING);
			break;
///////////////////////////////
			case PM_ORCUS:
				(void)mongets(mtmp, WAN_DEATH); /* the Wand of Orcus */
			break;
			case PM_MALCANTHET:
				(void) mongets(mtmp, BULLWHIP);
				(void) mongets(mtmp, ELVEN_CLOAK);
				(void) mongets(mtmp, POT_EXTRA_HEALING);
				(void) mongets(mtmp, POT_EXTRA_HEALING);
			break;
			case PM_GRAZ_ZT:
				otmp = mksobj(TWO_HANDED_SWORD, TRUE, FALSE);
				otmp = oname(otmp, artiname(ART_DOOMSCREAMER));
				otmp->blessed = FALSE;
				otmp->cursed = FALSE;
				(void) mpickobj(mtmp,otmp);
				(void) mongets(mtmp, AMULET_OF_REFLECTION);
				(void) mongets(mtmp, POT_EXTRA_HEALING);
				(void) mongets(mtmp, POT_EXTRA_HEALING);
			break;
///////////////////////////////
			case PM_BAEL:
				otmp = mksobj(TWO_HANDED_SWORD, TRUE, FALSE);
				otmp = oname(otmp, artiname(ART_GENOCIDE));		
				otmp->spe = 9;
				curse(otmp);
				otmp->oerodeproof = TRUE;
				(void) mpickobj(mtmp, otmp);
				(void) mongets(mtmp, TWO_HANDED_SWORD);
				(void) mongets(mtmp, GAUNTLETS_OF_POWER);
				(void) mongets(mtmp, POT_EXTRA_HEALING);
			break;
		    case PM_DISPATER:
//				(void)mongets(mtmp, WAN_STRIKING);
//				(void) mongets(mtmp, MACE);
				otmp = mksobj(MACE, TRUE, FALSE);
				otmp = oname(otmp, artiname(ART_ROD_OF_DIS));		
				otmp->blessed = FALSE;
				otmp->cursed = TRUE;
				otmp->spe = 2;
				otmp->oerodeproof = TRUE;
				(void) mpickobj(mtmp, otmp);
				(void) mongets(mtmp, SPE_CHARM_MONSTER);
				(void) mongets(mtmp, SCR_TAMING);
				(void) mongets(mtmp, SCR_TAMING);
				(void) mongets(mtmp, POT_EXTRA_HEALING);
			break;
		    case PM_MAMMON:
//				(void)mongets(mtmp, WAN_STRIKING);
				otmp = mksobj(SHORT_SWORD, TRUE, FALSE);
				otmp = oname(otmp, artiname(ART_AVERICE));		
				otmp->blessed = FALSE;
				otmp->cursed = TRUE;
				otmp->spe = 3;
				otmp->oerodeproof = TRUE;
				(void) mpickobj(mtmp, otmp);
				(void) mongets(mtmp, POT_PARALYSIS);
				(void) mongets(mtmp, SPE_DETECT_TREASURE);
				(void) mongets(mtmp, SCR_GOLD_DETECTION);
				(void) mongets(mtmp, SCR_GOLD_DETECTION);
				(void) mongets(mtmp, POT_EXTRA_HEALING);
			break;
			case PM_FIRENNA:
				(void)mongets(mtmp, WAN_DIGGING);
				(void) mongets(mtmp, POT_EXTRA_HEALING);
			break;
			case PM_BELIAL:
				otmp = mksobj(TRIDENT, TRUE, FALSE);
				otmp = oname(otmp, artiname(ART_FIRE_OF_HEAVEN));		
				otmp->blessed = FALSE;
				otmp->cursed = TRUE;
				otmp->spe = 4;
				otmp->oerodeproof = TRUE;
				(void) mpickobj(mtmp, otmp);
				(void)mongets(mtmp, WAN_DIGGING);
				(void) mongets(mtmp, POT_EXTRA_HEALING);
				(void) mongets(mtmp, POT_EXTRA_HEALING);
			break;
////////////////////////////////////////
			case PM_LEVIATHAN:
				otmp = mksobj(DUNCE_CAP, TRUE, FALSE);
				otmp = oname(otmp, artiname(ART_DIADEM_OF_AMNESIA));		
				otmp->blessed = FALSE;
				otmp->cursed = TRUE;
				otmp->spe = 5;
				otmp->oerodeproof = TRUE;
				(void) mpickobj(mtmp, otmp);
				(void)mongets(mtmp, WAN_CANCELLATION);
				(void)mongets(mtmp, POT_AMNESIA);
				(void)mongets(mtmp, POT_AMNESIA);
				(void)mongets(mtmp, POT_AMNESIA);
				(void) mongets(mtmp, POT_EXTRA_HEALING);
			break;
			case PM_DAUGHTER_LILITH:
				(void)mongets(mtmp, OILSKIN_CLOAK);
				otmp = mksobj(SILVER_DAGGER, TRUE, FALSE);
				otmp = oname(otmp, artiname(ART_THUNDER_S_VOICE));		
				otmp->blessed = FALSE;
				otmp->cursed = TRUE;
				otmp->spe = 6;
				otmp->oerodeproof = TRUE;
				(void) mpickobj(mtmp, otmp);
				(void) mongets(mtmp, POT_EXTRA_HEALING);
			break;
			case PM_MOTHER_LILITH:
				(void)mongets(mtmp, CLOAK_OF_MAGIC_RESISTANCE);
				(void)mongets(mtmp, OILSKIN_CLOAK);
				otmp = mksobj(ATHAME, TRUE, FALSE);
				otmp = oname(otmp, artiname(ART_SERPENT_S_TOOTH));		
				otmp->blessed = FALSE;
				otmp->cursed = TRUE;
				otmp->spe = 6;
				otmp->oerodeproof = TRUE;
				(void) mpickobj(mtmp, otmp);
				(void) mongets(mtmp, POT_EXTRA_HEALING);
			break;
			case PM_CRONE_LILITH:
				(void)mongets(mtmp, BOULDER);
				(void)mongets(mtmp, BOULDER);
				(void)mongets(mtmp, BOULDER);
				(void)mongets(mtmp, QUARTERSTAFF);
				otmp = mksobj(UNICORN_HORN, TRUE, FALSE);
				otmp = oname(otmp, artiname(ART_UNBLEMISHED_SOUL));		
				otmp->blessed = FALSE;
				otmp->cursed = TRUE;
				otmp->spe = 6;
				otmp->oerodeproof = TRUE;
				(void) mpickobj(mtmp, otmp);
				(void)mongets(mtmp, CRYSTAL_BALL);
				(void) mongets(mtmp, POT_EXTRA_HEALING);
				(void) mongets(mtmp, POT_EXTRA_HEALING);
			break;
			case PM_BAALZEBUB:
				otmp = mksobj(LONG_SWORD, TRUE, FALSE);
				otmp = oname(otmp, artiname(ART_WRATH_OF_HEAVEN));		
				otmp->blessed = FALSE;
				otmp->cursed = TRUE;
				otmp->spe = 7;
				otmp->oerodeproof = TRUE;
				(void) mpickobj(mtmp, otmp);

				otmp = mksobj(HELM_OF_TELEPATHY, TRUE, FALSE);
				otmp = oname(otmp, artiname(ART_ALL_SEEING_EYE_OF_THE_FLY));		
				otmp->blessed = FALSE;
				otmp->cursed = TRUE;
				otmp->spe = 7;
				otmp->oerodeproof = TRUE;
				(void) mpickobj(mtmp, otmp);
				(void)mongets(mtmp, RIN_FREE_ACTION);
				(void) mongets(mtmp, POT_EXTRA_HEALING);
			break;
			case PM_MEPHISTOPHELES:
				otmp = mksobj(RANSEUR, TRUE, FALSE);
				otmp = oname(otmp, artiname(ART_COLD_SOUL));		
				otmp->blessed = FALSE;
				otmp->cursed = TRUE;
				otmp->spe = 8;
				otmp->oerodeproof = TRUE;
				(void) mpickobj(mtmp, otmp);
				(void)mongets(mtmp, HELM_OF_BRILLIANCE);
				(void) mongets(mtmp, POT_EXTRA_HEALING);
			break;
			case PM_BAALPHEGOR:
				otmp = mksobj(IRON_BAR, TRUE, FALSE);
				otmp = oname(otmp, artiname(ART_SCEPTRE_OF_THE_FROZEN_FLOO));		
				otmp->blessed = FALSE;
				otmp->cursed = TRUE;
				otmp->spe = 8;
				otmp->oerodeproof = TRUE;
				(void) mpickobj(mtmp, otmp);
				(void)mongets(mtmp, AMULET_OF_REFLECTION);
				(void) mongets(mtmp, POT_FULL_HEALING);
				(void) mongets(mtmp, POT_EXTRA_HEALING);
			break;
/////////////////////////////////////////
			case PM_ASMODEUS:
//				(void)mongets(mtmp, WAN_COLD);
//				(void)mongets(mtmp, WAN_FIRE);
				(void)mongets(mtmp, ROBE);
				(void)mongets(mtmp, SPEED_BOOTS);
				(void)mongets(mtmp, SCR_CHARGING);
				(void) mongets(mtmp, POT_FULL_HEALING);
				(void) mongets(mtmp, POT_FULL_HEALING);
				(void) mongets(mtmp, POT_EXTRA_HEALING);
				(void) mongets(mtmp, POT_EXTRA_HEALING);
			break;
////////////////////////////////////////
			case PM_CHAOS:
				mtmp->mextra[1] = 0;
				{
				struct	monst *mlocal;
				/* create special stuff; can't use mongets */
				otmp = mksobj(CRYSTAL_BALL, TRUE, FALSE);
				otmp = oname(otmp, artiname(ART_BLACK_CRYSTAL));		
				bless(otmp);
				otmp->oerodeproof = TRUE;
				(void) mpickobj(mtmp, otmp);

				otmp = mksobj(SKELETON_KEY, TRUE, FALSE);
				otmp = oname(otmp, artiname(ART_THIRD_KEY_OF_CHAOS));
				otmp->blessed = FALSE;
				otmp->cursed = FALSE;
				(void) mpickobj(mtmp,otmp);

				mlocal = makemon(&mons[PM_KRAKEN], mtmp->mx, mtmp->my, MM_ADJACENTOK);
				otmp = mksobj(CRYSTAL_BALL, TRUE, FALSE);
				otmp = oname(otmp, artiname(ART_WATER_CRYSTAL));		
				bless(otmp);
				otmp->oerodeproof = TRUE;
				(void) mpickobj(mlocal, otmp);

				mlocal = makemon(&mons[PM_MARILITH], mtmp->mx, mtmp->my, MM_ADJACENTOK);
				otmp = mksobj(CRYSTAL_BALL, TRUE, FALSE);
				otmp = oname(otmp, artiname(ART_FIRE_CRYSTAL));		
				bless(otmp);
				otmp->oerodeproof = TRUE;
				(void) mpickobj(mlocal, otmp);

				mlocal = makemon(&mons[PM_TIAMAT], mtmp->mx, mtmp->my, MM_ADJACENTOK);
				otmp = mksobj(CRYSTAL_BALL, TRUE, FALSE);
				otmp = oname(otmp, artiname(ART_AIR_CRYSTAL));		
				bless(otmp);
				otmp->oerodeproof = TRUE;
				(void) mpickobj(mlocal, otmp);

				mlocal = makemon(&mons[PM_LICH], mtmp->mx, mtmp->my, MM_ADJACENTOK);
				otmp = mksobj(CRYSTAL_BALL, TRUE, FALSE);
				otmp = oname(otmp, artiname(ART_EARTH_CRYSTAL));		
				bless(otmp);
				otmp->oerodeproof = TRUE;
				(void) mpickobj(mlocal, otmp);
				}
			break;
			case PM_GREAT_CTHULHU:
//				otmp = mksobj(UNIVERSAL_KEY, TRUE, FALSE);
//				otmp = oname(otmp, artiname(ART_SILVER_KEY));
//				otmp->blessed = FALSE;
//				otmp->cursed = FALSE;
//				(void) mpickobj(mtmp,otmp);
			break;

			}
		break;
		case S_EYE:
			if(ptr == &mons[PM_AXUS]){
				struct obj *otmp = mksobj(SKELETON_KEY, TRUE, FALSE);
				otmp = oname(otmp, artiname(ART_FIRST_KEY_OF_LAW));
				otmp->blessed = FALSE;
				otmp->cursed = FALSE;
				(void) mpickobj(mtmp,otmp);
			}
		break;
		case S_GOLEM:
			if(ptr == &mons[PM_CENTER_OF_ALL]){
				struct obj *otmp = mksobj(BARDICHE, TRUE, FALSE);
				otmp->blessed = TRUE;
				otmp->cursed = FALSE;
				otmp->spe = 8;
				(void) mpickobj(mtmp,otmp);

				otmp = mksobj(SKELETON_KEY, TRUE, FALSE);
				otmp = oname(otmp, artiname(ART_FIRST_KEY_OF_NEUTRALITY));
				otmp->blessed = FALSE;
				otmp->cursed = FALSE;
				(void) mpickobj(mtmp,otmp);
				return;
			}
			if(ptr == &mons[PM_ARSENAL]){
				struct obj *otmp = mksobj(SKELETON_KEY, TRUE, FALSE);
				otmp = oname(otmp, artiname(ART_SECOND_KEY_OF_LAW));
				otmp->blessed = FALSE;
				otmp->cursed = FALSE;
				(void) mpickobj(mtmp,otmp);
			}
		break;
	    default:
		break;
	}

	/* ordinary soldiers rarely have access to magic (or gold :-) */
	if (ptr == &mons[PM_SOLDIER] && rn2(13)) return;

	if ((int) mtmp->m_lev > rn2(50))
		(void) mongets(mtmp, rnd_defensive_item(mtmp));
	if ((int) mtmp->m_lev > rn2(100))
		(void) mongets(mtmp, rnd_misc_item(mtmp));
#ifndef GOLDOBJ
	if (likes_gold(ptr) && !mtmp->mgold && !rn2(5))
		mtmp->mgold =
		      (long) d(level_difficulty(), mtmp->minvent ? 5 : 10);
#else
	if (likes_gold(ptr) && !findgold(mtmp->minvent) && !rn2(5))
		mkmonmoney(mtmp, (long) d(level_difficulty(), mtmp->minvent ? 5 : 10));
#endif
}

/* Note: for long worms, always call cutworm (cutworm calls clone_mon) */
struct monst *
clone_mon(mon, x, y)
struct monst *mon;
xchar x, y;	/* clone's preferred location or 0 (near mon) */
{
	coord mm;
	struct monst *m2;

	/* may be too weak or have been extinguished for population control */
	if (mon->mhp <= 1 || (mvitals[monsndx(mon->data)].mvflags & G_EXTINCT))
	    return (struct monst *)0;

	if (x == 0) {
	    mm.x = mon->mx;
	    mm.y = mon->my;
	    if (!enexto(&mm, mm.x, mm.y, mon->data) || MON_AT(mm.x, mm.y))
		return (struct monst *)0;
	} else if (!isok(x, y)) {
	    return (struct monst *)0;	/* paranoia */
	} else {
	    mm.x = x;
	    mm.y = y;
	    if (MON_AT(mm.x, mm.y)) {
		if (!enexto(&mm, mm.x, mm.y, mon->data) || MON_AT(mm.x, mm.y))
		    return (struct monst *)0;
	    }
	}
	m2 = newmonst(0);
	*m2 = *mon;			/* copy condition of old monster */
	m2->nmon = fmon;
	fmon = m2;
	m2->m_id = flags.ident++;
	if (!m2->m_id) m2->m_id = flags.ident++;	/* ident overflowed */
	m2->mx = mm.x;
	m2->my = mm.y;

	m2->minvent = (struct obj *) 0; /* objects don't clone */
	m2->mleashed = FALSE;
#ifndef GOLDOBJ
	m2->mgold = 0L;
#endif
	/* Max HP the same, but current HP halved for both.  The caller
	 * might want to override this by halving the max HP also.
	 * When current HP is odd, the original keeps the extra point.
	 */
	m2->mhpmax = mon->mhpmax;
	m2->mhp = mon->mhp / 2;
	mon->mhp -= m2->mhp;

	/* since shopkeepers and guards will only be cloned if they've been
	 * polymorphed away from their original forms, the clone doesn't have
	 * room for the extra information.  we also don't want two shopkeepers
	 * around for the same shop.
	 */
	if (mon->isshk) m2->isshk = FALSE;
	if (mon->isgd) m2->isgd = FALSE;
	if (mon->ispriest) m2->ispriest = FALSE;
	m2->mxlth = 0;
	place_monster(m2, m2->mx, m2->my);
	if (emits_light(m2->data))
	    new_light_source(m2->mx, m2->my, emits_light(m2->data),
			     LS_MONSTER, (genericptr_t)m2);
	if (m2->mnamelth) {
	    m2->mnamelth = 0; /* or it won't get allocated */
	    m2 = christen_monst(m2, NAME(mon));
	} else if (mon->isshk) {
	    m2 = christen_monst(m2, shkname(mon));
	}

	/* not all clones caused by player are tame or peaceful */
	if (!flags.mon_moving) {
	    if (mon->mtame)
		m2->mtame = rn2(max(2 + u.uluck, 2)) ? mon->mtame : 0;
	    else if (mon->mpeaceful)
		m2->mpeaceful = rn2(max(2 + u.uluck, 2)) ? 1 : 0;
	}

	newsym(m2->mx,m2->my);	/* display the new monster */
	if (m2->mtame) {
	    struct monst *m3;

	    if (mon->isminion) {
		m3 = newmonst(sizeof(struct epri) + mon->mnamelth);
		*m3 = *m2;
		m3->mxlth = sizeof(struct epri);
		if (m2->mnamelth) Strcpy(NAME(m3), NAME(m2));
		*(EPRI(m3)) = *(EPRI(mon));
		replmon(m2, m3);
		m2 = m3;
	    } else {
		/* because m2 is a copy of mon it is tame but not init'ed.
		 * however, tamedog will not re-tame a tame dog, so m2
		 * must be made non-tame to get initialized properly.
		 */
		m2->mtame = 0;
		if ((m3 = tamedog(m2, (struct obj *)0)) != 0) {
		    m2 = m3;
		    *(EDOG(m2)) = *(EDOG(mon));
		}
	    }
	}
	set_malign(m2);

	return m2;
}

/*
 * Propagate a species
 *
 * Once a certain number of monsters are created, don't create any more
 * at random (i.e. make them extinct).  The previous (3.2) behavior was
 * to do this when a certain number had _died_, which didn't make
 * much sense.
 *
 * Returns FALSE propagation unsuccessful
 *         TRUE  propagation successful
 */
boolean
propagate(mndx, tally, ghostly)
int mndx;
boolean tally;
boolean ghostly;
{
	boolean result;
	uchar lim = mbirth_limit(mndx);
	boolean gone = (mvitals[mndx].mvflags & G_GONE); /* genocided or extinct */

	result = (((int) mvitals[mndx].born < lim) && !gone) ? TRUE : FALSE;

	/* if it's unique, don't ever make it again */
	if (mons[mndx].geno & G_UNIQ) mvitals[mndx].mvflags |= G_EXTINCT;

	if (mvitals[mndx].born < 255 && tally && (!ghostly || (ghostly && result)))
		 mvitals[mndx].born++;
	if ((int) mvitals[mndx].born >= lim && !(mons[mndx].geno & G_NOGEN) &&
		!(mvitals[mndx].mvflags & G_EXTINCT)) {
#if defined(DEBUG) && defined(WIZARD)
		if (wizard) pline("Automatically extinguished %s.",
					makeplural(mons[mndx].mname));
#endif
		mvitals[mndx].mvflags |= G_EXTINCT;
		reset_rndmonst(mndx);
	}
	return result;
}

/*
 * called with [x,y] = coordinates;
 *	[0,0] means anyplace
 *	[u.ux,u.uy] means: near player (if !in_mklev)
 *
 *	In case we make a monster group, only return the one at [x,y].
 */
struct monst *
makemon(ptr, x, y, mmflags)
register struct permonst *ptr;
register int	x, y;
register int	mmflags;
{
	register struct monst *mtmp;
	int mndx, mcham, ct, mitem, xlth;
	boolean anymon = (!ptr);
	boolean byyou = (x == u.ux && y == u.uy);
	boolean allow_minvent = ((mmflags & NO_MINVENT) == 0);
	boolean countbirth = ((mmflags & MM_NOCOUNTBIRTH) == 0);
	unsigned gpflags = (mmflags & MM_IGNOREWATER) ? MM_IGNOREWATER : 0;

	/* if caller wants random location, do it here */
	if(x == 0 && y == 0) {
		int tryct = 0;	/* careful with bigrooms */
		struct monst fakemon;

		fakemon.data = ptr;	/* set up for goodpos */
		do {
			x = rn1(COLNO-3,2);
			y = rn2(ROWNO);
		} while(!goodpos(x, y, ptr ? &fakemon : (struct monst *)0, gpflags) ||
			(!in_mklev && tryct++ < 50 && cansee(x, y)));
	} else if (byyou && !in_mklev) {
		coord bypos;

		if(enexto_core(&bypos, u.ux, u.uy, ptr, gpflags)) {
			x = bypos.x;
			y = bypos.y;
		} else
			return((struct monst *)0);
	}

	/* Does monster already exist at the position? */
	if(MON_AT(x, y)) {
		if ((mmflags & MM_ADJACENTOK) != 0) {
			coord bypos;
			if(enexto_core(&bypos, x, y, ptr, gpflags)) {
				if( !(mmflags & MM_ADJACENTSTRICT) || (
					bypos.x - x <= 1 && bypos.x - x >= -1 &&
					bypos.y - y <= 1 && bypos.y - y >= -1
				)){
					x = bypos.x;
					y = bypos.y;
				}
				else return((struct monst *) 0);
			} else
				return((struct monst *) 0);
		} else 
			return((struct monst *) 0);
	}

	if(ptr){
		mndx = monsndx(ptr);
		/* if you are to make a specific monster and it has
		   already been genocided, return */
		if (mvitals[mndx].mvflags & G_GENOD) return((struct monst *) 0);
#if defined(WIZARD) && defined(DEBUG)
		if (wizard && (mvitals[mndx].mvflags & G_EXTINCT))
		    pline("Explicitly creating extinct monster %s.",
			mons[mndx].mname);
#endif
	} else {
		/* make a random (common) monster that can survive here.
		 * (the special levels ask for random monsters at specific
		 * positions, causing mass drowning on the medusa level,
		 * for instance.)
		 */
		int tryct = 0;	/* maybe there are no good choices */
		struct monst fakemon;
		do {
			if(!(ptr = rndmonst())) {
#ifdef DEBUG
			    pline("Warning: no monster.");
#endif
			    return((struct monst *) 0);	/* no more monsters! */
			}
			fakemon.data = ptr;	/* set up for goodpos */
		} while(!goodpos(x, y, &fakemon, gpflags) && tryct++ < 50);
		mndx = monsndx(ptr);
	}
	(void) propagate(mndx, countbirth, FALSE);
	xlth = ptr->pxlth;
	if (mmflags & MM_EDOG) xlth += sizeof(struct edog);
	else if (mmflags & MM_EMIN) xlth += sizeof(struct emin);
	mtmp = newmonst(xlth);
	*mtmp = zeromonst;		/* clear all entries in structure */
	(void)memset((genericptr_t)mtmp->mextra, 0, xlth);
	mtmp->nmon = fmon;
	fmon = mtmp;
	mtmp->m_id = flags.ident++;
	if (!mtmp->m_id) mtmp->m_id = flags.ident++;	/* ident overflowed */
		set_mon_data(mtmp, ptr, 0);
	if (mtmp->data->msound == MS_LEADER)
	    quest_status.leader_m_id = mtmp->m_id;
	mtmp->mxlth = xlth;
	mtmp->mnum = mndx;

	mtmp->m_lev = adj_lev(ptr);
	if(mtmp->data == &mons[PM_CHOKHMAH_SEPHIRAH])
		mtmp->m_lev += u.chokhmah;
	if (is_golem(ptr)) {
	    mtmp->mhpmax = mtmp->mhp = golemhp(mndx);
	} else if (is_rider(ptr)) {
	    /* We want low HP, but a high mlevel so they can attack well */
	    mtmp->mhpmax = mtmp->mhp = d(10,8);
	} else if (ptr->mlevel > 49) {
	    /* "special" fixed hp monster
	     * the hit points are encoded in the mlevel in a somewhat strange
	     * way to fit in the 50..127 positive range of a signed character
	     * above the 1..49 that indicate "normal" monster levels */
//	    mtmp->mhpmax = mtmp->mhp = 2*(ptr->mlevel - 6);
	    mtmp->mhpmax = mtmp->mhp = 3*(ptr->mlevel - 6);
	    mtmp->m_lev = mtmp->mhp / 4;	/* approximation */
	} else if (ptr->mlet == S_DRAGON && mndx >= PM_GRAY_DRAGON) {
	    /* adult dragons */
	    mtmp->mhpmax = mtmp->mhp = (int) (In_endgame(&u.uz) ?
		(8 * mtmp->m_lev) : (4 * mtmp->m_lev + d((int)mtmp->m_lev, 4)));
	} else if (!mtmp->m_lev) {
	    mtmp->mhpmax = mtmp->mhp = rnd(4);
	} else {
	    mtmp->mhpmax = mtmp->mhp = d((int)mtmp->m_lev, 8);
	    if (is_home_elemental(ptr))
		mtmp->mhpmax = (mtmp->mhp *= 3);
	}

	if (is_female(ptr)) mtmp->female = TRUE;
	else if (is_male(ptr)) mtmp->female = FALSE;
	else mtmp->female = rn2(2);	/* ignored for neuters */

	if (In_sokoban(&u.uz) && !mindless(ptr))  /* know about traps here */
	    mtmp->mtrapseen = (1L << (PIT - 1)) | (1L << (HOLE - 1));
	if (ptr->msound == MS_LEADER)		/* leader knows about portal */
	    mtmp->mtrapseen |= (1L << (MAGIC_PORTAL-1));

	place_monster(mtmp, x, y);
	mtmp->mcansee = mtmp->mcanmove = TRUE;
	mtmp->mpeaceful = (mmflags & MM_ANGRY) ? FALSE : peace_minded(ptr);
	mtmp->mtraitor  = FALSE;
	mtmp->mcrazed  = FALSE;

	switch(ptr->mlet) {
		case S_MIMIC:
			set_mimic_sym(mtmp);
			break;
		case S_SPIDER:
		case S_SNAKE:
			if(in_mklev)
			    if(x && y)
				(void) mkobj_at(0, x, y, TRUE);
			if(hides_under(ptr) && OBJ_AT(x, y))
			    mtmp->mundetected = TRUE;
		break;
		case S_LIGHT:
		case S_ELEMENTAL:
			if (mndx == PM_STALKER || mndx == PM_BLACK_LIGHT) {
			    mtmp->perminvis = TRUE;
			    mtmp->minvis = TRUE;
			}
		break;
		case S_EYE:
			if (mndx == PM_QUINON){
				if(anymon){
					int num = 0;
					makemon(&mons[PM_TRITON], mtmp->mx, mtmp->my, MM_ADJACENTOK);
					num = rnd(6);
					for(num; num >= 0; num--) makemon(&mons[PM_DUTON], mtmp->mx, mtmp->my, MM_ADJACENTOK);
					m_initlgrp(makemon(&mons[PM_MONOTON], mtmp->mx, mtmp->my, MM_ADJACENTOK), mtmp->mx, mtmp->my);
				}
				makemon(&mons[PM_QUATON], mtmp->mx, mtmp->my, MM_ADJACENTOK);
			}
			if (mndx == PM_QUATON){
				if(anymon){
					int num = 0;
					num = rn1(6,5);
					for(num; num >= 0; num--) makemon(&mons[PM_MONOTON], mtmp->mx, mtmp->my, MM_ADJACENTOK);
				}
				makemon(&mons[PM_TRITON], mtmp->mx, mtmp->my, MM_ADJACENTOK);
			}
			if (mndx == PM_TRITON){
				if(anymon) m_initlgrp(makemon(&mons[PM_MONOTON], mtmp->mx, mtmp->my, MM_ADJACENTOK), mtmp->mx, mtmp->my);
				makemon(&mons[PM_DUTON], mtmp->mx, mtmp->my, MM_ADJACENTOK);
			}
			if (mndx == PM_DUTON){
				if(anymon && rn2(2)) m_initsgrp(makemon(&mons[PM_MONOTON], mtmp->mx, mtmp->my, MM_ADJACENTOK), mtmp->mx, mtmp->my);
				else makemon(&mons[PM_MONOTON], mtmp->mx, mtmp->my, MM_ADJACENTOK);
			}
/*			if(mndx == PM_VORLON_MISSILE){
				mtmp->mhpmax = 3;
				mtmp->mhp = 3;
			}
*/		break;
		case S_IMP:
			if (anymon && mndx == PM_LEGION_DEVIL_SERGEANT){
				int num = 0;
				num = 10;
				for(num; num > 0; num--) makemon(&mons[PM_LEGION_DEVIL_GRUNT], mtmp->mx, mtmp->my, MM_ADJACENTOK);
				num = 3;
				for(num; num > 0; num--) makemon(&mons[PM_LEGION_DEVIL_SOLDIER], mtmp->mx, mtmp->my, MM_ADJACENTOK);
			}
		if (anymon && mndx == PM_LEGION_DEVIL_CAPTAIN){
				int num = 0;
				num = 20;
				for(num; num > 0; num--) makemon(&mons[PM_LEGION_DEVIL_GRUNT], mtmp->mx, mtmp->my, MM_ADJACENTOK);
				num = 8;
				for(num; num > 0; num--) makemon(&mons[PM_LEGION_DEVIL_SOLDIER], mtmp->mx, mtmp->my, MM_ADJACENTOK);
				num = 2;
				for(num; num > 0; num--) makemon(&mons[PM_LEGION_DEVIL_SERGEANT], mtmp->mx, mtmp->my, MM_ADJACENTOK);
			}
		break;
	    case S_GIANT:
			if (anymon && mndx == PM_DEEPEST_ONE){
				int num = 0;
				num = rn1(3,3);
				for(num; num >= 0; num--) makemon(&mons[PM_DEEPER_ONE], mtmp->mx, mtmp->my, MM_ADJACENTOK);
				num = rn1(10,10);
				for(num; num >= 0; num--) makemon(&mons[PM_DEEP_ONE], mtmp->mx, mtmp->my, MM_ADJACENTOK);
			}
		break;
		case S_HUMANOID:
			if (anymon && mndx == PM_DEEPER_ONE){
				int num = 0;
				num = rn1(10,3);
				for(num; num >= 0; num--) makemon(&mons[PM_DEEP_ONE], mtmp->mx, mtmp->my, MM_ADJACENTOK);
			}
			if (anymon && mndx == PM_ELVENKING){
				int num = 0;
				num = rnd(2);
				for(num; num >= 0; num--) makemon(&mons[PM_ELF_LORD], mtmp->mx, mtmp->my, MM_ADJACENTOK);
				num = rn1(6,3);
				for(num; num >= 0; num--) makemon(&mons[PM_GREY_ELF], mtmp->mx, mtmp->my, MM_ADJACENTOK);
			}
		break;
		case S_FUNGUS:
			if (anymon && mndx == PM_MIGO_QUEEN){
				int num = 0;
				num = rn2(2);
				for(num; num >= 0; num--) makemon(&mons[PM_MIGO_PHILOSOPHER], mtmp->mx, mtmp->my, MM_ADJACENTOK);
				num = rn2(3)+3;
				for(num; num >= 0; num--) makemon(&mons[PM_MIGO_SOLDIER], mtmp->mx, mtmp->my, MM_ADJACENTOK);
				num = rn2(5)+5;
				for(num; num >= 0; num--) makemon(&mons[PM_MIGO_WORKER], mtmp->mx, mtmp->my, MM_ADJACENTOK);

			}
			if (mndx == PM_PHANTOM_FUNGUS) {
			    mtmp->perminvis = TRUE;
			    mtmp->minvis = TRUE;
			}
		break;
		case S_WRAITH:
			if (mndx == PM_PHANTASM){
			    mtmp->mhpmax = mtmp->mhp = d(1,10);
				if(rn2(2)) {
				    mtmp->perminvis = TRUE;
				    mtmp->minvis = TRUE;
				}
				if(!rn2(3)){
					mtmp->mspeed = MFAST;
					mtmp->permspeed = MFAST;
				}
				else if(rn2(2)){
					mtmp->mspeed = MSLOW;
					mtmp->permspeed = MSLOW;
				}
				else{
					mtmp->mspeed = 0;
					mtmp->permspeed = 0;
				}
				if(rn2(2)){
					mtmp->mflee = 1;
					mtmp->mfleetim = d(1,10);
				}
				if(rn2(2)){
					if(rn2(4)){
						mtmp->mcansee = 0;
						mtmp->mblinded = rn2(30);
					}
					else{
						mtmp->mcansee = 0;
						mtmp->mblinded = 0;
					}
				}
				if(rn2(2)){
					if(rn2(2)){
						mtmp->mconf = 1;
					}else if(rn2(2)){
						mtmp->mstun = 1;
					}else{
						mtmp->mcrazed = 1;
					}
				}
				if(rn2(2)){
					if(rn2(4)){
						mtmp->mcanmove = 0;
						mtmp->mfrozen = d(1,10);
					}else{
						mtmp->mcanmove = 0;
						mtmp->mfrozen = 0;
					}
				}
				if(!rn2(8)){
					mtmp->msleeping = 1;
				}
				if(!rn2(8)){
					mtmp->mpeaceful = 1;
				}
			}
		break;
		case S_TRAPPER:
			{
				int num=6;
				if(mndx==PM_METROID_QUEEN) 
					if(anymon) for(num; num >= 0; num--) makemon(&mons[PM_BABY_METROID], mtmp->mx, mtmp->my, MM_ADJACENTOK);
					else for(num; num >= 0; num--) makemon(&mons[PM_METROID], mtmp->mx, mtmp->my, MM_ADJACENTOK);
					}
		case S_VAMPIRE:
			{
//				if(mndx == PM_STAR_VAMPIRE){
//				    mtmp->minvis = TRUE;
//				    mtmp->perminvis = TRUE;
//				}
			}
		break;
		case S_BLOB:
			if (mndx == PM_SHOGGOTH){
				mtmp->mhpmax = 3*mtmp->mhpmax;
				mtmp->mhp = mtmp->mhpmax;
			}
		break;
	    case S_KETER:
			if (mndx == PM_CHOKHMAH_SEPHIRAH){
				mtmp->mhpmax = u.chokhmah*mtmp->mhpmax;
				mtmp->mhp = mtmp->mhpmax;
			}
		break;

		case S_EEL:
			if (is_pool(x, y))
			    mtmp->mundetected = TRUE;
			break;
		case S_LEPRECHAUN:
			mtmp->msleeping = 1;
			break;
		case S_JABBERWOCK:
		case S_NYMPH:
			if (rn2(5) && !u.uhave.amulet) mtmp->msleeping = 1;
		break;
		case S_ORC:
			if (Race_if(PM_ELF)) mtmp->mpeaceful = FALSE;
			if (anymon && mndx == PM_ORC_CAPTAIN){
				int num = 0;
				num = rn1(10,3);
				for(num; num >= 0; num--) makemon(&mons[PM_HILL_ORC], mtmp->mx, mtmp->my, MM_ADJACENTOK);
			}
			if (anymon && mndx == PM_ORC_SHAMAN){
				int num = 0;
				num = rnd(3);
				for(num; num >= 0; num--) makemon(&mons[PM_HILL_ORC], mtmp->mx, mtmp->my, MM_ADJACENTOK);
			}
		break;
		case S_UNICORN:
			if (is_unicorn(ptr) &&
					sgn(u.ualign.type) == sgn(ptr->maligntyp))
				mtmp->mpeaceful = TRUE;
//			if(mndx == PM_PINK_UNICORN){
//			    mtmp->minvis = TRUE;
//			    mtmp->perminvis = TRUE;
//			}
		break;
		case S_BAT:
			if (Inhell && is_bat(ptr))
			    mon_adjust_speed(mtmp, 2, (struct obj *)0);
		break;
		case S_GOLEM:
			if(mndx == PM_CENTER_OF_ALL){
			    mtmp->minvis = TRUE;
			    mtmp->perminvis = TRUE;
			}
		break;
		case S_PUDDING:
			if(mndx == PM_DARKNESS_GIVEN_HUNGER){
				mtmp->mhpmax = 3*mtmp->mhpmax;
				mtmp->mhp = mtmp->mhpmax;
			    mtmp->minvis = TRUE;
			    mtmp->perminvis = TRUE;
			}
		break;
		case S_DEMON:
//			pline("%d\n",mtmp->mhpmax);
			if(mndx == PM_JUIBLEX){
				mtmp->mhpmax = 4*mtmp->mhpmax;
				mtmp->mhp = mtmp->mhpmax;
			}
			else if(mndx == PM_ZUGGTMOY){
				mtmp->mhpmax = 3*mtmp->mhpmax;
				mtmp->mhp = mtmp->mhpmax;
			}
			else if(mndx == PM_ASMODEUS){
				mtmp->mhpmax = 2*mtmp->mhpmax;
				mtmp->mhp = mtmp->mhpmax;
			}
			else if(mndx == PM_CHAOS){
				mtmp->mhpmax = 10*mtmp->mhpmax;
				mtmp->mhp = mtmp->mhpmax;
			}
			else if(mndx == PM_DEMOGORGON){ 
				mtmp->mhpmax = 2*mtmp->mhpmax;
				mtmp->mhp = mtmp->mhpmax;
			}
			if(mndx == PM_ANCIENT_OF_DEATH){
			    mtmp->minvis = TRUE;
			    mtmp->perminvis = TRUE;
			}
//			pline("%d\n",mtmp->mhpmax);
		break;
	}
	if ((ct = emits_light(mtmp->data)) > 0)
		new_light_source(mtmp->mx, mtmp->my, ct,
				 LS_MONSTER, (genericptr_t)mtmp);
	mitem = 0;	/* extra inventory item for this monster */

	if ((mcham = pm_to_cham(mndx)) != CHAM_ORDINARY) {
		/* If you're protected with a ring, don't create
		 * any shape-changing chameleons -dgk
		 */
		if (Protection_from_shape_changers)
			mtmp->cham = CHAM_ORDINARY;
		else {
			mtmp->cham = mcham;
			(void) newcham(mtmp, rndmonst(), FALSE, FALSE);
		}
	} else if (mndx == PM_WIZARD_OF_YENDOR) {
		mtmp->iswiz = TRUE;
		flags.no_of_wizards++;
		if (flags.no_of_wizards == 1 && Is_earthlevel(&u.uz))
			mitem = SPE_DIG;
	} else if (mndx == PM_DJINNI) {
		flags.djinni_count++;
	} else if (mndx == PM_GHOST) {
		flags.ghost_count++;
		if (!(mmflags & MM_NONAME))
			mtmp = christen_monst(mtmp, rndghostname());
	} else if (mndx == PM_VLAD_THE_IMPALER) {
		mitem = CANDELABRUM_OF_INVOCATION;
	} else if (mndx == PM_CROESUS) {
		mitem = TWO_HANDED_SWORD;
	} else if (ptr->msound == MS_NEMESIS) {
		mitem = BELL_OF_OPENING;
	} else if (mndx == PM_PESTILENCE) {
		mitem = POT_SICKNESS;
	}
	if (mitem && allow_minvent) (void) mongets(mtmp, mitem);

	if(in_mklev) {
		if(((is_ndemon(ptr)) ||
		    (mndx == PM_WUMPUS) ||
		    (mndx == PM_LONG_WORM) ||
		    (mndx == PM_GIANT_EEL)) && !u.uhave.amulet && rn2(5))
			mtmp->msleeping = TRUE;
	} else {
		if(byyou) {
			newsym(mtmp->mx,mtmp->my);
			set_apparxy(mtmp);
		}
	}
	if(is_dprince(ptr) && ptr->msound == MS_BRIBE) {
	    mtmp->mpeaceful = mtmp->minvis = mtmp->perminvis = 1;
	    mtmp->mavenge = 0;
	    if (uwep && (
			uwep->oartifact == ART_EXCALIBUR
			|| uwep->oartifact == ART_ROD_OF_SEVEN_PARTS
			|| uwep->oartifact == ART_LANCEA_LONGINI
		) ) mtmp->mpeaceful = mtmp->mtame = FALSE;
	}
#ifndef DCC30_BUG
	if (mndx == PM_LONG_WORM && (mtmp->wormno = get_wormno()) != 0)
#else
	/* DICE 3.0 doesn't like assigning and comparing mtmp->wormno in the
	 * same expression.
	 */
	if (mndx == PM_LONG_WORM &&
		(mtmp->wormno = get_wormno(), mtmp->wormno != 0))
#endif
	{
	    /* we can now create worms with tails - 11/91 */
	    initworm(mtmp, rn2(5));
	    if (count_wsegs(mtmp)) place_worm_tail_randomly(mtmp, x, y);
	}
	set_malign(mtmp);		/* having finished peaceful changes */
	if(u.uevent.uaxus_foe && (mndx <= PM_QUINON || mndx >= PM_MONOTON)){
		mtmp->mpeaceful = mtmp->mtame = FALSE;
	}
	if(anymon) {
	    if ((ptr->geno & G_SGROUP) && rn2(2)) {
		m_initsgrp(mtmp, mtmp->mx, mtmp->my);
	    } else if (ptr->geno & G_LGROUP) {
		if(rn2(3))  m_initlgrp(mtmp, mtmp->mx, mtmp->my);
		else	    m_initsgrp(mtmp, mtmp->mx, mtmp->my);
	    }
	}

	if (allow_minvent) {
	    if(is_armed(ptr))
		m_initweap(mtmp);	/* equip with weapons / armor */
	    m_initinv(mtmp);  /* add on a few special items incl. more armor */
	    m_dowear(mtmp, TRUE);
	} else {
	    /* no initial inventory is allowed */
	    if (mtmp->minvent) discard_minvent(mtmp);
	    mtmp->minvent = (struct obj *)0;    /* caller expects this */
	}
	if ((ptr->mflags3 & M3_WAITMASK) && !(mmflags & MM_NOWAIT)) {
		if (ptr->mflags3 & M3_WAITFORU)
			mtmp->mstrategy |= STRAT_WAITFORU;
		if (ptr->mflags3 & M3_CLOSE)
			mtmp->mstrategy |= STRAT_CLOSE;
	}

	if (!in_mklev)
	    newsym(mtmp->mx,mtmp->my);	/* make sure the mon shows up */

	return(mtmp);
}

int
mbirth_limit(mndx)
int mndx;
{
	/* assert(MAXMONNO < 255); */
	return (mndx == PM_NAZGUL ? 9 : mndx == PM_ERINYS ? 3 : mndx == PM_METROID ? 21 : mndx == PM_ALPHA_METROID ? 45 
		: mndx == PM_GAMMA_METROID ? 48 : mndx == PM_ZETA_METROID ? 9 : mndx == PM_OMEGA_METROID ? 12
		: mndx == PM_METROID_QUEEN ? 3 : mndx == PM_ARGENTUM_GOLEM ? 8 : mndx == PM_ALHOON ? 2 
		: mndx == PM_CENTER_OF_ALL ? 1 : mndx == PM_SOLDIER ? 250 : mndx == PM_SOLDIER_ANT ? 250 : MAXMONNO); 
}

/* used for wand/scroll/spell of create monster */
/* returns TRUE iff you know monsters have been created */
boolean
create_critters(cnt, mptr)
int cnt;
struct permonst *mptr;		/* usually null; used for confused reading */
{
	coord c;
	int x, y;
	struct monst *mon;
	boolean known = FALSE;
#ifdef WIZARD
	boolean ask = wizard;
#endif

	while (cnt--) {
#ifdef WIZARD
	    if (ask) {
		if (create_particular()) {
		    known = TRUE;
		    continue;
		}
		else ask = FALSE;	/* ESC will shut off prompting */
	    }
#endif
	    x = u.ux,  y = u.uy;
	    /* if in water, try to encourage an aquatic monster
	       by finding and then specifying another wet location */
	    if (!mptr && u.uinwater && enexto(&c, x, y, &mons[PM_GIANT_EEL]))
		x = c.x,  y = c.y;

	    mon = makemon(mptr, x, y, NO_MM_FLAGS);
	    if (mon && canspotmon(mon)) known = TRUE;
	}
	return known;
}

#endif /* OVL1 */
#ifdef OVL0

STATIC_OVL boolean
uncommon(mndx)
int mndx;
{
	if (mons[mndx].geno & (G_NOGEN | G_UNIQ)) return TRUE;
	if (mvitals[mndx].mvflags & G_GONE) return TRUE;
	if (Inhell)
		return(mons[mndx].maligntyp > A_NEUTRAL);
	else
		return((mons[mndx].geno & G_HELL) != 0);
}

/*
 *	shift the probability of a monster's generation by
 *	comparing the dungeon alignment and monster alignment.
 *	return an integer in the range of 0-5.
 */
STATIC_OVL int
align_shift(ptr)
register struct permonst *ptr;
{
    static NEARDATA long oldmoves = 0L;	/* != 1, starting value of moves */
    static NEARDATA s_level *lev;
    register int alshift;

    if(oldmoves != moves) {
	lev = Is_special(&u.uz);
	oldmoves = moves;
    }
    switch((lev) ? lev->flags.align : dungeons[u.uz.dnum].flags.align) {
    default:	/* just in case */
    case AM_NONE:	alshift = 0;
			break;
    case AM_LAWFUL:	alshift = (ptr->maligntyp+20)/(2*ALIGNWEIGHT);
			break;
    case AM_NEUTRAL:	alshift = (20 - abs(ptr->maligntyp))/ALIGNWEIGHT;
			break;
    case AM_CHAOTIC:	alshift = (-(ptr->maligntyp-20))/(2*ALIGNWEIGHT);
			break;
    }
    return alshift;
}

static NEARDATA struct {
	int choice_count;
	char mchoices[SPECIAL_PM];	/* value range is 0..127 */
} rndmonst_state = { -1, {0} };

/* select a random monster type */
struct permonst *
rndmonst()
{
	register struct permonst *ptr;
	register int mndx, ct;

	if((u.uevent.sum_entered || !rn2(100)) && !(mvitals[PM_CENTER_OF_ALL].mvflags & G_EXTINCT) && !rn2(100)){
	    return &mons[PM_CENTER_OF_ALL]; /*center of all may be created at any time, but is much more likely after
												entering sum of all */
	}

	if (u.uz.dnum == quest_dnum && rn2(7) && (ptr = qt_montype()) != 0)
	    return ptr;

	if (u.uz.dnum == neutral_dnum && 
		(on_level(&rlyeh_level,&u.uz) ||  on_level(&sum_of_all_level,&u.uz) || on_level(&gatetown_level,&u.uz)))
	    if(!in_mklev) return neutral_montype();
		else return (struct permonst *)0;/*NOTE: ugly method to stop monster generation durning level creation, since I cant find a better way*/

	if (u.uz.dnum == chaos_dnum)
	    return chaos_montype();

	if (u.uz.dnum == law_dnum)
	    return law_montype();

	if (u.uz.dnum == mines_dnum){
		int roll = d(1,10);
		switch(roll){
			case 1:	case 2: case 3: case 4: return mkclass(S_GNOME, 0); break;
			case 5:	case 6: return &mons[PM_DWARF]; break;
			default: break; //proceed with normal generation
		}
	}

	if (u.uz.dnum == tower_dnum)
		return rn2(10) ? mkclass(S_ZOMBIE, 0) : mkclass(S_VAMPIRE, 0);

	if (u.uz.dnum == tomb_dnum)
		return rn2(2) ? mkclass(S_ZOMBIE, 0) : mkclass(S_MUMMY, 0);

	if (u.uz.dnum == temple_dnum)
		return rn2(4) ? mkclass(S_ZOMBIE, 0) : mkclass(S_BLOB, 0);

	if (u.uz.dnum == sea_dnum)
		return rn2(3) ? &mons[PM_JELLYFISH] : rn2(2) ? &mons[PM_SHARK] : &mons[PM_GIANT_EEL];

	if (rndmonst_state.choice_count < 0) {	/* need to recalculate */
	    int zlevel, minmlev, maxmlev;
	    boolean elemlevel;
#ifdef REINCARNATION
	    boolean upper;
#endif

	    rndmonst_state.choice_count = 0;
	    /* look for first common monster */
	    for (mndx = LOW_PM; mndx < SPECIAL_PM; mndx++) {
		if (!uncommon(mndx)) break;
		rndmonst_state.mchoices[mndx] = 0;
	    }		
	    if (mndx == SPECIAL_PM) {
		/* evidently they've all been exterminated */
#ifdef DEBUG
		pline("rndmonst: no common mons!");
#endif
		return (struct permonst *)0;
	    } /* else `mndx' now ready for use below */
	    zlevel = level_difficulty();
	    /* determine the level of the weakest monster to make. */
	    minmlev = zlevel / 6;
	    /* determine the level of the strongest monster to make. */
	    maxmlev = (zlevel + u.ulevel) / 2;
#ifdef REINCARNATION
	    upper = Is_rogue_level(&u.uz);
#endif
	    elemlevel = In_endgame(&u.uz) && !Is_astralevel(&u.uz);

		if(u.hod && !rn2(10) && rn2(50+u.hod) > 50){
			u.hod--;
			if(!tooweak(PM_HOD_SEPHIRAH, minmlev)){
				return &mons[PM_HOD_SEPHIRAH];
			}
			else u.keter++;
		}
		if(u.gevurah && rn2(1000) < max(u.gevurah/4 + 1, 50)){
			if(!tooweak(PM_GEVURAH_SEPHIRAH, minmlev)){
				return &mons[PM_GEVURAH_SEPHIRAH];
			}
			else{
				u.gevurah -= 4;
				u.keter++;
				return &mons[PM_CHOKHMAH_SEPHIRAH];
			}
		}
		if(u.keter && rn2(1000) < max(u.keter, 100)){
			u.chokhmah++;
			return &mons[PM_CHOKHMAH_SEPHIRAH];
		}
/*
 *	Find out how many monsters exist in the range we have selected.
 */
	    /* (`mndx' initialized above) */
	    for ( ; mndx < SPECIAL_PM; mndx++) {
		ptr = &mons[mndx];
		rndmonst_state.mchoices[mndx] = 0;
		if (tooweak(mndx, minmlev) || toostrong(mndx, maxmlev))
		    continue;
#ifdef REINCARNATION
		if (upper && !isupper(def_monsyms[(int)(ptr->mlet)])) continue;
#endif
		if (elemlevel && wrong_elem_type(ptr)) continue;
		if (uncommon(mndx)) continue;
		if (Inhell && (ptr->geno & G_NOHELL)) continue;
		ct = (int)(ptr->geno & G_FREQ) + align_shift(ptr);
		if (ct < 0 || ct > 127)
		    panic("rndmonst: bad count [#%d: %d]", mndx, ct);
		rndmonst_state.choice_count += ct;
		rndmonst_state.mchoices[mndx] = (char)ct;
	    }
/*
 *	    Possible modification:  if choice_count is "too low",
 *	    expand minmlev..maxmlev range and try again.
 */
	} /* choice_count+mchoices[] recalc */

	if (rndmonst_state.choice_count <= 0) {
	    /* maybe no common mons left, or all are too weak or too strong */
#ifdef DEBUG
	    Norep("rndmonst: choice_count=%d", rndmonst_state.choice_count);
#endif
	    return (struct permonst *)0;
	}

/*
 *	Now, select a monster at random.
 */
	ct = rnd(rndmonst_state.choice_count);
	for (mndx = LOW_PM; mndx < SPECIAL_PM; mndx++)
	    if ((ct -= (int)rndmonst_state.mchoices[mndx]) <= 0) break;

	if (mndx == SPECIAL_PM || uncommon(mndx)) {	/* shouldn't happen */
	    impossible("rndmonst: bad `mndx' [#%d]", mndx);
	    return (struct permonst *)0;
	}
	return &mons[mndx];
}

/* called when you change level (experience or dungeon depth) or when
   monster species can no longer be created (genocide or extinction) */
void
reset_rndmonst(mndx)
int mndx;	/* particular species that can no longer be created */
{
	/* cached selection info is out of date */
	if (mndx == NON_PM) {
	    rndmonst_state.choice_count = -1;	/* full recalc needed */
	} else if (mndx < SPECIAL_PM) {
	    rndmonst_state.choice_count -= rndmonst_state.mchoices[mndx];
	    rndmonst_state.mchoices[mndx] = 0;
	} /* note: safe to ignore extinction of unique monsters */
}

#endif /* OVL0 */
#ifdef OVL1

/*	The routine below is used to make one of the multiple types
 *	of a given monster class.  The second parameter specifies a
 *	special casing bit mask to allow the normal genesis
 *	masks to be deactivated.  Returns 0 if no monsters
 *	in that class can be made.
 */

struct permonst *
mkclass(class,spc)
char	class;
int	spc;
{
	register int	first, last, num = 0;
	int maxmlev, mask = (G_NOGEN | G_UNIQ) & ~spc;

	maxmlev = level_difficulty() >> 1;
	if(class < 1 || class >= MAXMCLASSES) {
	    impossible("mkclass called with bad class!");
	    return((struct permonst *) 0);
	}
/*	Assumption #1:	monsters of a given class are contiguous in the
 *			mons[] array.
 */
	for (first = LOW_PM; first < SPECIAL_PM; first++)
	    if (mons[first].mlet == class) break;
	if (first == SPECIAL_PM) return (struct permonst *) 0;

	for (last = first;
		last < SPECIAL_PM && mons[last].mlet == class; last++)
	    if (!(mvitals[last].mvflags & G_GONE) && !(mons[last].geno & mask)
					&& !is_placeholder(&mons[last])) {
		/* consider it */
		if(num && toostrong(last, maxmlev) &&
		   monstr[last] != monstr[last-1] && rn2(2)) break;
		num += mons[last].geno & G_FREQ;
	    }

	if(!num) return((struct permonst *) 0);

/*	Assumption #2:	monsters of a given class are presented in ascending
 *			order of strength.
 */
	for(num = rnd(num); num > 0; first++)
	    if (!(mvitals[first].mvflags & G_GONE) && !(mons[first].geno & mask)
					&& !is_placeholder(&mons[first])) {
		/* skew towards lower value monsters at lower exp. levels */
		num -= mons[first].geno & G_FREQ;
		if (num && adj_lev(&mons[first]) > (u.ulevel*2)) {
		    /* but not when multiple monsters are same level */
		    if (mons[first].mlevel != mons[first+1].mlevel)
			num--;
		}
	    }
	first--; /* correct an off-by-one error */

	return(&mons[first]);
}

int
adj_lev(ptr)	/* adjust strength of monsters based on u.uz and u.ulevel */
register struct permonst *ptr;
{
	int	tmp, tmp2;

	if (ptr == &mons[PM_WIZARD_OF_YENDOR]) {
		/* does not depend on other strengths, but does get stronger
		 * every time he is killed
		 */
		tmp = ptr->mlevel + mvitals[PM_WIZARD_OF_YENDOR].died;
		if (tmp > 49) tmp = 49;
		return tmp;
	}

	if((tmp = ptr->mlevel) > 49) return(50); /* "special" demons/devils */
	tmp2 = (level_difficulty() - tmp);
	if(tmp2 < 0) tmp--;		/* if mlevel > u.uz decrement tmp */
	else tmp += (tmp2 / 5);		/* else increment 1 per five diff */

	tmp2 = (u.ulevel - ptr->mlevel);	/* adjust vs. the player */
	if(tmp2 > 0) tmp += (tmp2 / 4);		/* level as well */

	tmp2 = (3 * ((int) ptr->mlevel))/ 2;	/* crude upper limit */
	if (tmp2 > 49) tmp2 = 49;		/* hard upper limit */
	return((tmp > tmp2) ? tmp2 : (tmp > 0 ? tmp : 0)); /* 0 lower limit */
}

#endif /* OVL1 */
#ifdef OVLB

struct permonst *
grow_up(mtmp, victim)	/* `mtmp' might "grow up" into a bigger version */
struct monst *mtmp, *victim;
{
	int oldtype, newtype, max_increase, cur_increase,
	    lev_limit, hp_threshold;
	struct permonst *ptr = mtmp->data;

	/* monster died after killing enemy but before calling this function */
	/* currently possible if killing a gas spore */
	if (mtmp->mhp <= 0)
	    return ((struct permonst *)0);

	if(mtmp->m_lev > 50 || ptr == &mons[PM_CHAOS]) return ((struct permonst *)0);
	/* note:  none of the monsters with special hit point calculations
	   have both little and big forms */
	oldtype = monsndx(ptr);
	newtype = little_to_big(oldtype);
	if (newtype == PM_PRIEST && mtmp->female) newtype = PM_PRIESTESS;

	/* growth limits differ depending on method of advancement */
	if (victim) {		/* killed a monster */
	    /*
	     * The HP threshold is the maximum number of hit points for the
	     * current level; once exceeded, a level will be gained.
	     * Possible bug: if somehow the hit points are already higher
	     * than that, monster will gain a level without any increase in HP.
	     */
	    hp_threshold = mtmp->m_lev * 8;		/* normal limit */
	    if (!mtmp->m_lev)
		hp_threshold = 4;
	    else if (is_golem(ptr))	/* strange creatures */
		hp_threshold = ((mtmp->mhpmax / 10) + 1) * 10 - 1;
	    else if (is_home_elemental(ptr) || 
			ptr == &mons[PM_DARKNESS_GIVEN_HUNGER] ||
			ptr == &mons[PM_SHOGGOTH]
		) hp_threshold *= 3;
		else if(ptr == &mons[PM_CHOKHMAH_SEPHIRAH]) hp_threshold *= u.chokhmah;
	    lev_limit = 3 * (int)ptr->mlevel / 2;	/* same as adj_lev() */
	    /* If they can grow up, be sure the level is high enough for that */
	    if (oldtype != newtype && mons[newtype].mlevel > lev_limit)
		lev_limit = (int)mons[newtype].mlevel;
	    /* number of hit points to gain; unlike for the player, we put
	       the limit at the bottom of the next level rather than the top */
	    max_increase = rnd((int)victim->m_lev + 1);
	    if (mtmp->mhpmax + max_increase > hp_threshold + 1)
		max_increase = max((hp_threshold + 1) - mtmp->mhpmax, 0);
	    cur_increase = (max_increase > 1) ? rn2(max_increase) : 0;
	} else {
	    /* a gain level potion or wraith corpse; always go up a level
	       unless already at maximum (49 is hard upper limit except
	       for demon lords, who start at 50 and can't go any higher) */
	    max_increase = cur_increase = rnd(8);
	    hp_threshold = 0;	/* smaller than `mhpmax + max_increase' */
	    lev_limit = 50;		/* recalc below */
	}

	mtmp->mhpmax += max_increase;
	mtmp->mhp += cur_increase;
	if (mtmp->mhpmax <= hp_threshold)
	    return ptr;		/* doesn't gain a level */

	if (is_mplayer(ptr) || ptr == &mons[PM_BYAKHEE]) lev_limit = 30;	/* same as player */
	else if (lev_limit < 5) lev_limit = 5;	/* arbitrary */
	else if (lev_limit > 49) lev_limit = (ptr->mlevel > 49 ? ptr->mlevel : 49);

	if ((int)++mtmp->m_lev >= mons[newtype].mlevel && newtype != oldtype) {
	    ptr = &mons[newtype];
	    if (mvitals[newtype].mvflags & G_GENOD) {	/* allow G_EXTINCT */
		if (sensemon(mtmp))
		    pline("As %s grows up into %s, %s %s!", mon_nam(mtmp),
			an(ptr->mname), mhe(mtmp),
			nonliving(ptr) ? "expires" : "dies");
		set_mon_data(mtmp, ptr, -1);	/* keep mvitals[] accurate */
		mondied(mtmp);
		return (struct permonst *)0;
	    }
	    set_mon_data(mtmp, ptr, 1);		/* preserve intrinsics */
	    newsym(mtmp->mx, mtmp->my);		/* color may change */
	    lev_limit = (int)mtmp->m_lev;	/* never undo increment */
		if(newtype == PM_METROID_QUEEN && mtmp->mtame){
			struct monst *baby;
			int tnum = d(1,6);
			int i = 0;
			mtmp->mtame = 0;
			mtmp->mpeaceful = 1;
			for(i; i < 6; i++){
				baby = makemon(&mons[PM_BABY_METROID], mtmp->mx, mtmp->my, MM_ADJACENTOK);
				if(tnum--) tamedog(baby,(struct obj *) 0);
			}
		}
	}
	/* sanity checks */
	if ((int)mtmp->m_lev > lev_limit) {
	    mtmp->m_lev--;	/* undo increment */
	    /* HP might have been allowed to grow when it shouldn't */
	    if (mtmp->mhpmax == hp_threshold + 1) mtmp->mhpmax--;
	}
//	if( !(monsndx(mtmp)<PM_DJINNI && monsndx(mtmp)>PM_BALROG) ){
		if (mtmp->mhpmax > 50*8) mtmp->mhpmax = 50*8;	  /* absolute limit */
		if (mtmp->mhp > mtmp->mhpmax) mtmp->mhp = mtmp->mhpmax;
//	}
	return ptr;
}

#endif /* OVLB */
#ifdef OVL1

int
mongets(mtmp, otyp)
register struct monst *mtmp;
register int otyp;
{
	register struct obj *otmp;
	int spe;

	if (!otyp) return 0;
	otmp = mksobj(otyp, TRUE, FALSE);
	if (otmp) {
	    if (mtmp->data->mlet == S_DEMON) {
		/* demons never get blessed objects */
		if (otmp->blessed) curse(otmp);
	    } else if(is_lminion(mtmp)) {
		/* lawful minions don't get cursed, bad, or rusting objects */
		otmp->cursed = FALSE;
		if(otmp->spe < 0) otmp->spe = 0;
		otmp->oerodeproof = TRUE;
	    } else if(is_mplayer(mtmp->data) && is_sword(otmp)) {
		otmp->spe = (3 + rn2(4));
	    }

	    if(otmp->otyp == CANDELABRUM_OF_INVOCATION) {
		otmp->spe = 0;
		otmp->age = 0L;
		otmp->lamplit = FALSE;
		otmp->blessed = otmp->cursed = FALSE;
	    } else if (otmp->otyp == BELL_OF_OPENING) {
		otmp->blessed = otmp->cursed = FALSE;
	    } else if (otmp->otyp == SPE_BOOK_OF_THE_DEAD) {
		otmp->blessed = FALSE;
		otmp->cursed = TRUE;
	    }

	    /* leaders don't tolerate inferior quality battle gear */
	    if (is_prince(mtmp->data)) {
		if (otmp->oclass == WEAPON_CLASS && otmp->spe < 1)
		    otmp->spe = 1;
		else if (otmp->oclass == ARMOR_CLASS && otmp->spe < 0)
		    otmp->spe = 0;
	    }

	    spe = otmp->spe;
	    (void) mpickobj(mtmp, otmp);	/* might free otmp */
	    return(spe);
	} else return(0);
}

#endif /* OVL1 */
#ifdef OVLB

int
golemhp(type)
int type;
{
	switch(type) {
		case PM_STRAW_GOLEM: return 20;
		case PM_PAPER_GOLEM: return 20;
		case PM_ROPE_GOLEM: return 30;
		case PM_LEATHER_GOLEM: return 40;
		case PM_GOLD_GOLEM: return 40;
		case PM_WOOD_GOLEM: return 50;
		case PM_FLESH_GOLEM: return 40;
		case PM_SPELL_GOLEM: return 20;
//		case PM_SAURON_THE_IMPRISONED: return 45;
		case PM_CLAY_GOLEM: return 50;
		case PM_TREASURY_GOLEM: return 60;
		case PM_STONE_GOLEM: return 60;
		case PM_GLASS_GOLEM: return 60;
		case PM_ARGENTUM_GOLEM: return 70;
		case PM_IRON_GOLEM: return 80;
		case PM_ARSENAL: return 88;
		case PM_CENTER_OF_ALL: return 88;
		case PM_RETRIEVER: return 120;
//		case PM_HEAD_OF_THE_UNKNOWN_GOD: return 65;
//		case PM_BODY_OF_THE_UNKNOWN_GOD: return 65;
//		case PM_LEGS_OF_THE_UNKNOWN_GOD: return 65;
//		case PM_EYE_OF_THE_UNKNOWN_GOD: return 65;
//		case PM_SERVANT_OF_THE_UNKNOWN_GOD: return 65;
//		case PM_SCOURGE_OF_THE_UNKNOWN_GOD: return 65;
		default: return 0;
	}
}

#endif /* OVLB */
#ifdef OVL1

/*
 *	Alignment vs. yours determines monster's attitude to you.
 *	( some "animal" types are co-aligned, but also hungry )
 */
boolean
peace_minded(ptr)
register struct permonst *ptr;
{
	int mndx = monsndx(ptr);
	aligntyp mal = ptr->maligntyp, ual = u.ualign.type;

	if ((mndx <= PM_QUINON && mndx >= PM_MONOTON) && sgn(mal) == sgn(ual)){
		if(!u.uevent.uaxus_foe){
			return TRUE;
		} else return FALSE;
	}
	if (always_peaceful(ptr)) return TRUE;
	if (always_hostile(ptr) && (u.uz.dnum != law_dnum || !is_social_insect(ptr)
		|| (!on_level(&arcadia1_level,&u.uz) && !on_level(&arcadia2_level,&u.uz) && !on_level(&arcadia3_level,&u.uz) && !on_level(&arcward_level,&u.uz))
		)) return FALSE;
	if (ptr->msound == MS_LEADER || ptr->msound == MS_GUARDIAN)
		return TRUE;
	if (ptr->msound == MS_NEMESIS)	return FALSE;

	if (race_peaceful(ptr)) return TRUE;
	if (race_hostile(ptr)) return FALSE;

	/* the monster is hostile if its alignment is different from the
	 * player's */
	if (sgn(mal) != sgn(ual)) return FALSE;

	/* Negative monster hostile to player with Amulet. */
	if (mal < A_NEUTRAL && u.uhave.amulet) return FALSE;

	/* minions are hostile to players that have strayed at all */
	if (is_minion(ptr)) return((boolean)(u.ualign.record >= 0));

	/* Last case:  a chance of a co-aligned monster being
	 * hostile.  This chance is greater if the player has strayed
	 * (u.ualign.record negative) or the monster is not strongly aligned.
	 */
	return((boolean)(!!rn2(16 + (u.ualign.record < -15 ? -15 : u.ualign.record)) &&
		!!rn2(2 + abs(mal))));
}

/* Set malign to have the proper effect on player alignment if monster is
 * killed.  Negative numbers mean it's bad to kill this monster; positive
 * numbers mean it's good.  Since there are more hostile monsters than
 * peaceful monsters, the penalty for killing a peaceful monster should be
 * greater than the bonus for killing a hostile monster to maintain balance.
 * Rules:
 *   it's bad to kill peaceful monsters, potentially worse to kill always-
 *	peaceful monsters
 *   it's never bad to kill a hostile monster, although it may not be good
 */
void
set_malign(mtmp)
struct monst *mtmp;
{
	schar mal = mtmp->data->maligntyp;
	boolean coaligned;

	if (mtmp->ispriest || mtmp->isminion) {
		/* some monsters have individual alignments; check them */
		if (mtmp->ispriest)
			mal = EPRI(mtmp)->shralign;
		else if (mtmp->isminion)
			mal = EMIN(mtmp)->min_align;
		/* unless alignment is none, set mal to -5,0,5 */
		/* (see align.h for valid aligntyp values)     */
		if(mal != A_NONE)
			mal *= 5;
	}

	coaligned = (sgn(mal) == sgn(u.ualign.type));
	if (mtmp->data->msound == MS_LEADER) {
		mtmp->malign = -20;
	} else if (mal == A_NONE) {
		if (mtmp->mpeaceful)
			mtmp->malign = 0;
		else
			mtmp->malign = 20;	/* really hostile */
	} else if (always_peaceful(mtmp->data)) {
		int absmal = abs(mal);
		if (mtmp->mpeaceful)
			mtmp->malign = -3*max(5,absmal);
		else
			mtmp->malign = 3*max(5,absmal); /* renegade */
	} else if (always_hostile(mtmp->data)) {
		int absmal = abs(mal);
		if (coaligned)
			mtmp->malign = 0;
		else
			mtmp->malign = max(5,absmal);
	} else if (coaligned) {
		int absmal = abs(mal);
		if (mtmp->mpeaceful)
			mtmp->malign = -3*max(3,absmal);
		else	/* renegade */
			mtmp->malign = max(3,absmal);
	} else	/* not coaligned and therefore hostile */
		mtmp->malign = abs(mal);
}

#endif /* OVL1 */
#ifdef OVLB

static NEARDATA char syms[] = {
	MAXOCLASSES, MAXOCLASSES+1, RING_CLASS, WAND_CLASS, WEAPON_CLASS,
	FOOD_CLASS, COIN_CLASS, SCROLL_CLASS, POTION_CLASS, ARMOR_CLASS,
	AMULET_CLASS, TOOL_CLASS, ROCK_CLASS, GEM_CLASS, SPBOOK_CLASS,
	S_MIMIC_DEF, S_MIMIC_DEF, S_MIMIC_DEF,
};

void
set_mimic_sym(mtmp)		/* KAA, modified by ERS */
register struct monst *mtmp;
{
	int typ, roomno, rt;
	unsigned appear, ap_type;
	int s_sym;
	struct obj *otmp;
	int mx, my;

	if (!mtmp) return;
	mx = mtmp->mx; my = mtmp->my;
	typ = levl[mx][my].typ;
					/* only valid for INSIDE of room */
	roomno = levl[mx][my].roomno - ROOMOFFSET;
	if (roomno >= 0)
		rt = rooms[roomno].rtype;
#ifdef SPECIALIZATION
	else if (IS_ROOM(typ))
		rt = OROOM,  roomno = 0;
#endif
	else	rt = 0;	/* roomno < 0 case for GCC_WARN */

	if (OBJ_AT(mx, my)) {
		ap_type = M_AP_OBJECT;
		appear = level.objects[mx][my]->otyp;
	} else if (IS_DOOR(typ) || IS_WALL(typ) ||
		   typ == SDOOR || typ == SCORR) {
		ap_type = M_AP_FURNITURE;
		/*
		 *  If there is a wall to the left that connects to this
		 *  location, then the mimic mimics a horizontal closed door.
		 *  This does not allow doors to be in corners of rooms.
		 */
		if (mx != 0 &&
			(levl[mx-1][my].typ == HWALL    ||
			 levl[mx-1][my].typ == TLCORNER ||
			 levl[mx-1][my].typ == TRWALL   ||
			 levl[mx-1][my].typ == BLCORNER ||
			 levl[mx-1][my].typ == TDWALL   ||
			 levl[mx-1][my].typ == CROSSWALL||
			 levl[mx-1][my].typ == TUWALL    ))
		    appear = S_hcdoor;
		else
		    appear = S_vcdoor;

		if(!mtmp->minvis || See_invisible)
		    block_point(mx,my);	/* vision */
	} else if (level.flags.is_maze_lev && rn2(2)) {
		ap_type = M_AP_OBJECT;
		appear = STATUE;
	} else if (roomno < 0) {
		ap_type = M_AP_OBJECT;
		appear = BOULDER;
		if(!mtmp->minvis || See_invisible)
		    block_point(mx,my);	/* vision */
	} else if (rt == ZOO || rt == VAULT) {
		ap_type = M_AP_OBJECT;
		appear = GOLD_PIECE;
	} else if (rt == DELPHI) {
		if (rn2(2)) {
			ap_type = M_AP_OBJECT;
			appear = STATUE;
		} else {
			ap_type = M_AP_FURNITURE;
			appear = S_fountain;
		}
	} else if (rt == TEMPLE) {
		ap_type = M_AP_FURNITURE;
		appear = S_altar;
	/*
	 * We won't bother with beehives, morgues, barracks, throne rooms
	 * since they shouldn't contain too many mimics anyway...
	 */
	} else if (rt >= SHOPBASE) {
		s_sym = get_shop_item(rt - SHOPBASE);
		if (s_sym < 0) {
			ap_type = M_AP_OBJECT;
			appear = -s_sym;
		} else {
			if (s_sym == RANDOM_CLASS)
				s_sym = syms[rn2((int)sizeof(syms)-2) + 2];
			goto assign_sym;
		}
	} else {
		s_sym = syms[rn2((int)sizeof(syms))];
assign_sym:
		if (s_sym >= MAXOCLASSES) {
			ap_type = M_AP_FURNITURE;
			appear = s_sym == MAXOCLASSES ? S_upstair : S_dnstair;
		} else if (s_sym == COIN_CLASS) {
			ap_type = M_AP_OBJECT;
			appear = GOLD_PIECE;
		} else {
			ap_type = M_AP_OBJECT;
			if (s_sym == S_MIMIC_DEF) {
				appear = STRANGE_OBJECT;
			} else {
				otmp = mkobj( (char) s_sym, FALSE );
				appear = otmp->otyp;
				/* make sure container contents are free'ed */
				obfree(otmp, (struct obj *) 0);
			}
		}
	}
	mtmp->m_ap_type = ap_type;
	mtmp->mappearance = appear;
}

/* release a monster from a bag of tricks */
void
bagotricks(bag)
struct obj *bag;
{
    if (!bag || bag->otyp != BAG_OF_TRICKS) {
	impossible("bad bag o' tricks");
    } else if (bag->spe < 1) {
	pline(nothing_happens);
    } else {
	boolean gotone = FALSE;
	int cnt = 1;

	consume_obj_charge(bag, TRUE);

	if (!rn2(23)) cnt += rn1(7, 1);
	while (cnt-- > 0) {
	    if (makemon((struct permonst *)0, u.ux, u.uy, NO_MM_FLAGS))
		gotone = TRUE;
	}
	if (gotone) makeknown(BAG_OF_TRICKS);
    }
}

#endif /* OVLB */

/*makemon.c*/
