/*	SCCS Id: @(#)engrave.c	3.4	2001/11/04	*/
/* Copyright (c) Stichting Mathematisch Centrum, Amsterdam, 1985. */
/* NetHack may be freely redistributed.  See license for details. */

#include "hack.h"
#include "lev.h"
#include "artifact.h"
#include <ctype.h>

STATIC_VAR NEARDATA struct engr *head_engr;
extern int NDECL(pick_ward);

#ifdef OVLB
/* random engravings */
static const char *random_mesg[] = {
	"Elbereth",
	/* trap engravings */
	"Vlad was here", "ad aerarium",
	/* take-offs and other famous engravings */
	"Owlbreath", "Galadriel",
	"Kilroy was here",
	"quis custodiet ipsos custodes?", /* Watchmen */
	"Bad Wolf", /* Dr. Who */
	"A.S. ->", "<- A.S.", /* Journey to the Center of the Earth */
	"You won't get it up the steps", /* Adventure */
	"Lasciate ogni speranza o voi ch'entrate.", /* Inferno */
	"Well Come", /* Prisoner */
	"We apologize for the inconvenience.", /* So Long... */
	"See you next Wednesday", /* Thriller */
	"notary sojak", /* Smokey Stover */
	"For a good time call 8?7-5309",
	"Please don't feed the animals.", /* Various zoos around the world */
	"Madam, in Eden, I'm Adam.", /* A palindrome */
	"Two thumbs up!", /* Siskel & Ebert */
	"Hello, World!", /* The First C Program */
	"Dare mighty things.", /* Moto of the Mars Science Laboratory */
	"I seem to be having tremendous difficulty with my lifestyle",
#ifdef MAIL
	"You've got mail!", /* AOL */
#endif
	"As if!", /* Clueless */
        "Arooo!  Werewolves of Yendor!", /* gang tag */
        "Dig for Victory here", /* pun, duh */
        "Don't go this way",
        "Gaius Julius Primigenius was here.  Why are you late?", /* pompeii */
        "Go left --->", "<--- Go right",
        "Haermund Hardaxe carved these runes", /* viking graffiti */
        "Here be dragons",
        "Need a light?  Come visit the Minetown branch of Izchak's Lighting Store!",
        "Save now, and do your homework!",
        "Snakes on the Astral Plane - Soon in a dungeon near you",
        "There was a hole here.  It's gone now.",
        "The Vibrating Square",
        "This is a pit!",
        "This is not the dungeon you are looking for.",
        "This square deliberately left blank.",
        "Warning, Exploding runes!",
        "Watch out, there's a gnome with a wand of death behind that door!",
        "X marks the spot",
        "X <--- You are here.",
        "You are the one millionth visitor to this place!  Please wait 200 turns for your wand of wishing.",
};

/* Note: most of these were contributed by members of the
	Nethack Usenet group, rec.games.roguelike.nethack */
static const char *haluMesg[] = {
	"Which way did he go, George, which way did he go?",
	"Until you stalk and overrun, you can't devour anyone.",
	"Fee.  Fie.  Foe.  Foo.",
	"Violence is the last refuge of the incompetent.",
	"Be vewy vewy quiet.  I'm hunting wabbits.",
	"Eh, what's up, doc?",
	"We prefer your extinction to the loss of our job.",
	"You're weird, sir.",
	"I do not know for what reason the enemy is pursuing you, but I perceive that he is, strange indeed though that seems to me.",
	"When it is falling on your head, then you are knowing it is a rock.",
	"The very best mail is made from corbomite.",
	"Woah, duude, look at all the colors.",
	"I'm bringing home a baby bumblebee.",
	"Now, if you'll permit me, I'll continue my character assassination unimpeded.",
	"Cry havoc, and let slip the dogs of war.",
	"Virtue needs some cheaper thrills.",
	"I just saw Absolom hanging in an oak tree!",
	"And I may do anything I wish as long as I squeeze?",
	"Marsey Dotesin Dosey Dotesin Diddley Damsey Divey.",
	"I knew I should have taken that left turn at Albuquerque.",
	"YOU BROKE MY SWORD!",
	"An atom-blaster is a good weapon, but it can point both ways.",
	"You may have already won ten million zorkmids.",
	"They say the satisfaction of teaching makes up for the lousy pay.",
	"Tharr she blows!",
	"Perhaps today is a good day to die.",
	"Sufferin' succotash.",
	"To succeed, planning alone is insufficient. One must improvise as well.",
	"I love it when a plan comes together.",
	"Very fond of rumpots, crackpots, and how are you Mr. Wilson?",
	"No matter how valuable it might be, any man's life is still worth more than any animal's.",
	"No clock for this.  Flyin' now.",
	"Of course, you realize, this means war.",
	"She's fast enough for you, old man. What's the cargo?",
	"There're few who haven't heard the name of Gurney Halleck.",
	"Do you like mayonnaise and corn on your pizza?",
	"I am that merry wanderer of the night.",
	"Don't try any games.  Remember I can see your face if you can't see mine.",
	"Ah hates rabbits.",
	"Captain, there be whales here!",
	"Mimesis is a plant.  Go Bucks!",
	"That's all we live for, isn't it?  For pleasure, for titillation?",
	"The only brew for the brave and true comes from the Green Dragon.",
	"There is an old Vulcan proverb: only Nixon could go to China.",
	"Yo, put me in a trebuchet and launch me to the stratosphere.",
	"Lorem ipsum dolor sit amet, consectetur adipisici elit, sed eiusmod tempor incidunt ut labore et dolore magna aliqua.",
	"Sola lingua bona lingua mortua est.",
	"Mairzy Doats And Dozy Doats And Liddle Lamzy Divey A kiddley divey too, wouldn't you?",
	"Madam, I may be drunk, but you are ugly, and in the morning I shall be sober.",
	"Outside of a dog, a book is a man's best friend.",
	"Inside of a dog, it's too dark to read." ,
	"You are superior in only one aspect. You are better at dying.",
	"No McFly ever amounted to anything in the history of Hill Valley!",
	"You've got to ask yourself one question: 'Do I feel lucky?' Well, do ya, punk?",
	"In this pearl it is hard to see a Strange Loop.",
	"That is because the Strange Loop is buried in the oyster -- the proof.",
	"I love the smell of napalm in the morning.",
	"Et tu, Brute?",
	"The conditions were difficult. Several contestants were maimed, but... I was triumphant. I won Champion Standing.",
	"We're not gonna take it. No, we ain't gonna take it. We're not gonna take it anymore.",
	"Absurd!  Your 'MU' is as silly as a cow's moo.",
	"Hasta la vista, baby.",
	"I see...  I see...  I see a lavender man...  bearing tidings.",
	"It is a sign of creative genius to reconcile the seemingly disparate.",
	"It pays to be obvious, especially if you have a reputation for subtlety.",
	"Bother. I've left my new torch in Narnia",
	"If you put into the machine wrong figures, will the right answers come out?",
	"I am not able rightly to apprehend the kind of confusion of ideas that could provoke such a question.",
	"Colorless green ideas sleep furiously",
	"We fear a few dwarf watercraft stewardesses were regarded as desegregated after a great Texas tweezer war.",
	"I'll kill him in Honolulu, you unhook my pink kimono.",
	"The clown can stay, but the Ferengi in the gorilla suit has got to go.",
	"All roads lead to Trantor, and that is where all stars end.",
	"It is a far, far better thing I do than I have ever done. It is a far, far better rest I go to than I have ever known.",
	"Yes, this is Network 23. The net-net-net-net-network that puts it right... where they want you to be!",
	"You're listening to the E.I.B Radio Network.",
	"We shall this day light such a candle, by God's grace, in England, as I trust shall never be put out.",
	"How about a little fire, scarecrow?",
	"And if you're the kind of person who parties with a bathtub full of pasta, I suspect you don't care much about cholesterol anyway.",
	"I AM A SEXY SHOELESS GOD OF WAR!!!",
	
	"All the world will be your enemy, Prince With A Thousand Enemies, and when they catch you they will kill you... but first they must catch you.",
	"This statement is false.",
	"Quod Erat Demonstrandum", "anno Domini", "per centum",
	"I was so surprised, I CLASSIFIED INFORMATION, I really did!",
	"[REDACTED]",
	"[DATA EXPUNGED]",
	"[DATA PLUNGED]",
	"[DATA EXPANDED]",
	"I am a toaster!",
	"I prepared Explosive Runes this morning.",
	"In any battle, there's always a level of force against which no tactics can succeed.",
	"1002: He is Not that Which He Says He Is.",
	"3001: Behinde the Eagle's Neste a grate Ash hath fellen.",
	"VI. You Shall Not Subject Your God To Market Forces!",
	"Short to long term memory impaired. Go to Robotics Building. Explain about Bowman's architecture. Write down everything.",
	"I SAW THE EYE. Over the horizon, like a rising sun!",
	"That's Science, isn't it? The perpetual horizon.\nBut we found a way, haha, we found a way around that!",
	"What would YOU give up to... to see it through?",
	"Fall in a more hardboiled manner.",
	"Since you are reading this, chances are you have already installed this game on your computer. If this is true, you have just participated in bringing about the end of the world. But don't beat yourself up about it.",
	"YOU HATE TIME TRAVEL YOU HATE TIME TRAVEL YOU HATE....",
	"It begins to dawn on you that everything you just did may have been a colossal waste of time.",
	"We will BUILD heroes!",
	"If you replace the working parts, you get a different machine.",
	"If you destroy the working parts, what you get is a broken machine."
};

const int wardStrokes[][7] = {
	{1,1,1, 1,1,1, 1},
	{21,21,21, 21,21,21, 21},
	{60,60,60, 60,60,60, 60},
	{2,2,2, 2,2,2, 2},
	{10,10,10, 10,10,10, 10},
	{12,12,12, 12,12,12, 12},
	{10,10,20, 10,20,10, 10},
	{6,10,8, 8,8,8, 8},
	{5,5,5, 5,5,5, 5},
	{8,8,8, 8,8,8, 8},
	{7,5,6, 7,5,4, 7},
	{10,10,10, 10,10,10, 10}
};

const int wardsAdded[][7] = {
	{1,1,1, 1,1,1, 1},
	{1,1,1, 1,1,1, 1},
	{1,1,1, 0,0,0, 0},
	{1,1,1, 1,0,0, 0},
	{1,1,1, 1,1,1, 1},
	{1,1,1, 1,1,1, 1},
	{1,1,2, 1,2,1, 0},
	{1,1,1, 1,1,0, 0},
	{1,1,1, 1,1,1, 1},
	{1,1,1, 1,1,1, 1},
	{1,1,1, 1,1,1, 1},
	{1,1,1, 1,1,1, 1}
};

//const int wardDesc[] = {0, 0,0,0, 0,0,0, 0,0,0, 0,1}
//const char * descOfWards[] = {"There is ", "The "}
//There is ? inscribed here.
const char * wardText[][7] =  {
	{"a one", "a two", "a three", "a four", "a five","a six", "a seven"},
	{"a heptagram", "a two-fold heptagram", "a three-fold heptagram", "a four-fold heptagram", 
		"a five-fold heptagram","a six-fold heptagram", "a seven-fold heptagram"},
	{"a Gorgoneion", "a pair of Gorgoneia", "a trio of Gorgoneia", "too many (4) Gorgoneia", 
		"too many (5) Gorgoneia", "too many (6) Gorgoneia", "too many (7) Gorgoneia"},
	{"a circle of Acheron", "a two-level circle of Acheron", "a three-level circle of Acheron", 
		"a four-level circle of Acheron", "too many (5) circles of Acheron",
		"too many (6) circles of Acheron", "too many (7) circles of Acheron"},
	{"a pentagram", "a two-fold pentagram", "a three-fold pentagram", "a four-fold pentagram", 
		"a five-fold pentagram","a six-fold pentagram", "a seven-fold pentagram"},
	{"a hexagram", "a two-fold hexagram", "a three-fold hexagram", "a four-fold hexagram", 
		"a five-fold hexagram","a six-fold hexagram", "a seven-fold hexagram"},
	{"a hamsa mark", "a pair of hamsa marks", "a three-handed hamsa mark", 
		"a four-handed hamsa mark", "a seven-handed hamsa mark","a six-handed hamsa mark", 
		"an overabundance of hamsa marks"},
	{"an elder sign", "a one-armed elder sign", "a two-armed elder sign", "a three-armed elder sign", 
		"a four-armed elder sign","a five-armed elder sign", "an extra-dimensional elder sign"},
	{"an elder elemental eye", "a two-lobed elder elemental eye", "a three-lobed elder elemental eye",
		"a four-lobed elder elemental eye", "a five-lobed elder elemental eye",
		"a six-lobed elder elmental eye", "a seven-lobed elder elemental eye"},
	{"a sign of the Scion Queen Mother", "a reinforced sign of the Scion Queen Mother", 
		"a doubly reinforced sign of the Scion Queen Mother", 
		"a triply reinforced sign of the Scion Queen Mother", 
		"a quadruply reinforced sign of the Scion Queen Mother",
		"a quintuply reinforced sign of the Scion Queen Mother", 
		"a sextuply reinforced sign of the Scion Queen Mother"},
	{"a one-hieroglyph partial cartouche of the Cat Lord", 
		"a two-hieroglyph partial cartouche of the Cat Lord", 
		"a three-hieroglyph partial cartouche of the Cat Lord", 
		"a five-hieroglyph partial cartouche of the Cat Lord", 
		"a six-hieroglyph partial cartouche of the Cat Lord",
		"a seven-hieroglyph partial cartouche of the Cat Lord", 
		"a nine-hieroglyph completed cartouche of the Cat Lord"},
	{"a rudamentary rendition of the wings of Garuda",
		"a almost passable rendition of the wings of Garuda", 
		"a passable rendition of the wings of Garuda",
		"a somewhat detailed rendition of the wings of Garuda",
		"a detailed rendition of the wings of Garuda",
		"a very detailed rendition of the wings of Garuda",
		"a splendid rendition of the wings of Garuda"},
};
const char * wardNames[][2] = {
	{"digit", "digits"},
	{"heptagram", "heptagrams"},
	{"Gorgoneion", "Gorgoneia"},
	{"circle of Acheron", "circles of Acheron"},
	{"pentagram", "pentagrams"},
	{"hexagram", "hexagrams"},
	{"hamsa mark", "hamsa marks"},
	{"elder sign", "elder signs"},
	{"elder elemental eye", "elder elemental eyes"},
	{"sign of the Scion Queen Mother", "signs of the Scion Queen Mother"},
	{"a hieroglyph", "hieroglyphs"},
	{"wing of Garuda", "wings of Garuda"},
};
/* There is %s drawn here */
const char * haluWard[] =  {
	"", /* 0 index */
	"a series of disconected lines", /* nondescript*/

	/*DnD*/
	"a cerulean weeping-willow", /* it's magic. Unlike the others, this one works. Keep in sync with engrave.h!*/

	/* books */
	"a set of holy horns", "a Summoning Dark mine-sign", "a Long Dark mine-sign",
	"a Following Dark mine-sign", "a Closing Dark mine-sign", "an Opening Dark mine-sign",
	"a Breathing Dark mine-sign", "a Speaking Dark mine-sign", "a Catching Dark mine-sign",
	"a Secret Dark mine-sign", "a Calling Dark mine-sign", "a Waiting Dark mine-sign",
	"a florid crest dominated by a double-headed bat",
	"a Guarding Dark mine-sign", "the mark of the Caller of Eight", /* Discworld */
	"a lidless eye", /* Lord of the Rings */
	"a white tree", /* Gondor, Lord of the Rings */
	"a yellow sign", /* The King in Yellow */
	"a triangle enclosing a circle and bisected by a line", /* Harry Potter */
	"a set of three trefoils, the lower most inverted", /* describes the three of clubs. Too Many Magicians*/
	"a Trump of Doom", "a Sign of Chaos", "a Pattern of Amber",
	
	/* Not quite */
	"a heptagenarian",
	"a mimir",
	"an octogram",
	"a pentagrain",
	"a circle of da Vinci",
	"a hand making a rude gesture",
	"a junior sign",
	"a childish impure eye",
	"a Sign of an Illegitimate Step-daughter",
	"a cenotaph of a catgirl",
	"a groovy rendition of the wings of Gargula",
	
	"a Robotech Defense Force insignia", /*...Robotech*/
	"a Black Knights insignia", /* Code Geass */
	"an inverted triangle flanked by seven eyes", /* NGE */
	"a laughing man", /* Ghost in the Shell */
	"an alchemic array", "a human transmutation circle", /* Fullmetal Alchemist */
	"a triangle composed of three smaller triangles", "an eye and single tear", 
	"a circle enclosing four swirling lines", "a flame inside a circle",
	"a snowflake within a circle", "an inverted triangle with a dot above each face, enclosed by a circle",
	"a sign resembling an eyeless yin-yang", 
	"a circle surrounding a triangle of dots and another of triangels",/*Zelda*/
	"a setting (rising?) sun", /* Dresden Codak */
	"an asymmetric, stylized arrowhead, point upwards", /* Star Trek*/
	"a set of three blades, the top blade straight, the left curved down, the right curved up",
	"a Sharuan Mindharp", /* Star Wars expanded universe */
	"a winged blade of light", /* Jedi Order symbol */
	"a screw attack symbol",
	"more dakka",
	
	/* Planescape */
	"a symbol of pain",
	"a symbol of torment",
	"a circle enclosing two colliding arrows",
	"a pair of triangles, drawn tip to tip,",
	"a stylized beast",
	"a triangle crowned by a single line",
	"a simple image of many mountains",
	"a sketch of a shining diamond",
	"a tree-rune",
	"an eight-toothed gear",
	"a random scribble",
	"a square with two small handles on opposite sides",
	"a square enclosing a spiral",
	"an eye with three inverted crosses",
	"an infinity symbol crossed by a burning downwards arrow",
	"a set of four nested triangles",
	"a watchful eye blocking an upward arrow",
	"a pitchfork stabbing the ground",
	
	"a heart pierced through with an arrow",
	"a broken heart",
	"a skull and crossed bones",
	"a bad situation",
	"a zorkmid",
	
	"a diagram of the bridges of Konigsberg",
	"a glider from the Game of Life",

	"a hand-mirror of Aphrodite",
	"a shield and spear of Ares", /* alchemy/male/female */
	
	"a black moon lilith astrological sign",

	"a window", /* op-sys*/
	"a no symbol",
	"a test pattern",
	"a work of modern art",
	"a flag of Neverland",
	"a hyped-up duck dressed in a sailor's shirt and hat",
	"a set of three circles in the shape of a mouse's head",
	"a stylized fan-shaped seashell",
	"a pair of arches meeting to form an \"M\"",
	"a Swoosh mark",
	"a set of five interlocked rings",
	"a running man", /* Exit */
	"a one-and-zero", /* Power toggle */
	"a thick soup of mist", "a pattern of squared circles", 
	"a void", "a notable lack of images", "a stark absence of pictures", "nothing much",
	"a convergence of parallel lines", "a sphere", /* How did you manage that? */
	"a yin-yang", "a taijitu",/* Taoist */
	"a hand of Eris", /* Discordian */
	"an ichthus", "a Cross", /* Christian*/
	"a fish with legs", "a fat fish", "a fish with tentacles, legs, and wings",
		/* ichthus parodies/derivitives: darwin, buddha, and Cthulhu. */
	"a set of seven concentric circles",
	"a left-handed trefoil knot",
	"a triskelion", /* Ancient Symbol */
	"a wheel with eight spokes", /* Budhism */
	"a rough circle enclosing an A", /* Anarchy */
	"a Tree of Life", /* Kabbalah */
	"a winged oak",
	"a wheel cross", "a labyrinth",
	"an Eye of Providence", "a pyramid surmounted by an eye", /* Christian */
	"a one-way staircase",
	"an a encircled by its own tail" /* meta */
};

char *
random_engraving(outbuf)
char *outbuf;
{
	const char *rumor;

	/* a random engraving may come from the "rumors" file,
	   or from the list above */
	if (!rn2(4) || !(rumor = getrumor(0, outbuf, TRUE)) || !*rumor)
	    Strcpy(outbuf, random_mesg[rn2(SIZE(random_mesg))]);

	wipeout_text(outbuf, (int)(strlen(outbuf) / 4), 0);
	return outbuf;
}

/* Partial rubouts for engraving characters. -3. */
static const struct {
	char		wipefrom;
	const char *	wipeto;
} rubouts[] = {
	{'A', "^"},     {'B', "Pb["},   {'C', "("},     {'D', "|)["},
	{'E', "|FL[_"}, {'F', "|-"},    {'G', "C("},    {'H', "|-"},
	{'I', "|"},     {'K', "|<"},    {'L', "|_"},    {'M', "|"},
	{'N', "|\\"},   {'O', "C("},    {'P', "F"},     {'Q', "C("},
	{'R', "PF"},    {'T', "|"},     {'U', "J"},     {'V', "/\\"},
	{'W', "V/\\"},  {'Z', "/"},
	{'b', "|"},     {'d', "c|"},    {'e', "c"},     {'g', "c"},
	{'h', "n"},     {'j', "i"},     {'k', "|"},     {'l', "|"},
	{'m', "nr"},    {'n', "r"},     {'o', "c"},     {'q', "c"},
	{'w', "v"},     {'y', "v"},
	{':', "."},     {';', ","},
	{'0', "C("},    {'1', "|"},     {'6', "o"},     {'7', "/"},
	{'8', "3o"}
};

void
wipeout_text(engr, cnt, seed)
char *engr;
int cnt;
unsigned seed;		/* for semi-controlled randomization */
{
	char *s;
	int i, j, nxt, use_rubout, lth = (int)strlen(engr);

	if (lth && cnt > 0) {
	    while (cnt--) {
		/* pick next character */
		if (!seed) {
		    /* random */
		    nxt = rn2(lth);
		    use_rubout = rn2(4);
		} else {
		    /* predictable; caller can reproduce the same sequence by
		       supplying the same arguments later, or a pseudo-random
		       sequence by varying any of them */
		    nxt = seed % lth;
		    seed *= 31,  seed %= (BUFSZ-1);
		    use_rubout = seed & 3;
		}
		s = &engr[nxt];
		if (*s == ' ') continue;

		/* rub out unreadable & small punctuation marks */
		if (index("?.,'`-|_", *s)) {
		    *s = ' ';
		    continue;
		}

		if (!use_rubout)
		    i = SIZE(rubouts);
		else
		    for (i = 0; i < SIZE(rubouts); i++)
			if (*s == rubouts[i].wipefrom) {
			    /*
			     * Pick one of the substitutes at random.
			     */
			    if (!seed)
				j = rn2(strlen(rubouts[i].wipeto));
			    else {
				seed *= 31,  seed %= (BUFSZ-1);
				j = seed % (strlen(rubouts[i].wipeto));
			    }
			    *s = rubouts[i].wipeto[j];
			    break;
			}

		/* didn't pick rubout; use '?' for unreadable character */
		if (i == SIZE(rubouts)) *s = '?';
	    }
	}

	/* trim trailing spaces */
	while (lth && engr[lth-1] == ' ') engr[--lth] = 0;
}

void
wipeout_ward(ep, cnt, seed)
struct engr *ep;
int cnt;
unsigned seed;		/* for semi-controlled randomization */
{
	char *s;
	int i, j, nxt, lth, clth, slth, dlth, plth;
	lth = 0;
	clth = slth = dlth = plth = 0;
	j = 0;
	if(ep->halu_ward){
		clth = ep->complete_wards * 10;
		slth = ep->scuffed_wards * 10;
		dlth = ep->degraded_wards * 10;
		plth = ep->partial_wards * 10;
		lth = clth+slth+dlth+plth;
	}
	else{
		i = ep->complete_wards;
		while(i-- > 0){
			lth += wardStrokes[ep->ward_id][j];
			clth += wardStrokes[ep->ward_id][j];
			j++;
		}
		i = ep->scuffed_wards;
		slth = clth;
		while(i-- > 0){
			lth += wardStrokes[ep->ward_id][j] * .9;
			slth += wardStrokes[ep->ward_id][j] * .9;
			j++;
		}
		i = ep->degraded_wards;
		dlth = slth;
		while(i-- > 0){
			lth += wardStrokes[ep->ward_id][j] * .75;
			dlth += wardStrokes[ep->ward_id][j] * .75;
			j++;
		}
		i = ep->partial_wards;
		plth = dlth;
		while(i-- > 0){
			lth += wardStrokes[ep->ward_id][j] * .5;
			plth += wardStrokes[ep->ward_id][j] * .5;
			j++;
		}
	}
	if (ep->ward_id && cnt > 0) {
	    while (ep->ward_id && cnt--) {
			/* pick next character */
			if (!seed) {
				/* random */
				nxt = rn2(lth);
			} else {
				/* predictable; caller can reproduce the same sequence by
				   supplying the same arguments later, or a pseudo-random
				   sequence by varying any of them */
				nxt = seed % lth;
				seed *= 31,  seed %= (BUFSZ-1);
			}
			if(nxt < clth){
				ep->complete_wards--;
				ep->scuffed_wards++;
			}
			else if(nxt < slth){
				ep->scuffed_wards--;
				ep->degraded_wards++;
			}
			else if(nxt < dlth){
				ep->degraded_wards--;
				ep->partial_wards++;
			}
			else if(nxt < plth) ep->partial_wards--;
			
			lth = 0;
			clth = slth = dlth = plth = 0;
			j = 0;
			if(ep->halu_ward){
				clth = ep->complete_wards * 10;
				slth = ep->scuffed_wards * 10;
				dlth = ep->degraded_wards * 10;
				plth = ep->partial_wards * 10;
				lth = clth+slth+dlth+plth;
			}
			else{
				i = ep->complete_wards;
				while(i-- > 0){
					lth += wardStrokes[ep->ward_id][j];
					clth += wardStrokes[ep->ward_id][j];
					j++;
				}
				i = ep->scuffed_wards;
				while(i-- > 0){
					lth += wardStrokes[ep->ward_id][j] * .9;
					slth += wardStrokes[ep->ward_id][j] * .9;
					j++;
				}
				i = ep->degraded_wards;
				while(i-- > 0){
					lth += wardStrokes[ep->ward_id][j] * .75;
					dlth += wardStrokes[ep->ward_id][j] * .75;
					j++;
				}
				i = ep->partial_wards;
				while(i-- > 0){
					lth += wardStrokes[ep->ward_id][j] * .5;
					plth += wardStrokes[ep->ward_id][j] * .5;
					j++;
				}
			}
			if(!lth){
				ep->ward_id = 0;
				ep->ward_type = 0;
			}
		}
	}
}

boolean
can_reach_floor()
{
	return (boolean)(!u.uswallow &&
#ifdef STEED
			/* Restricted/unskilled riders can't reach the floor */
			!(u.usteed && P_SKILL(P_RIDING) < P_BASIC) &&
#endif
			 (!Levitation ||
			  Is_airlevel(&u.uz) || Is_waterlevel(&u.uz)));
}
#endif /* OVLB */
#ifdef OVL0

const char *
surface(x, y)
register int x, y;
{
	register struct rm *lev = &levl[x][y];

	if ((x == u.ux) && (y == u.uy) && u.uswallow &&
		is_animal(u.ustuck->data))
	    return "maw";
	else if (IS_AIR(lev->typ) && Is_airlevel(&u.uz))
	    return "air";
	else if (is_pool(x,y))
	    return (Underwater && !Is_waterlevel(&u.uz)) ? "bottom" : "water";
	else if (is_ice(x,y))
	    return "ice";
	else if (is_lava(x,y))
	    return "lava";
	else if (lev->typ == DRAWBRIDGE_DOWN)
	    return "bridge";
	else if(IS_ALTAR(levl[x][y].typ))
	    return "altar";
	else if(IS_GRAVE(levl[x][y].typ))
	    return "headstone";
	else if(IS_FOUNTAIN(levl[x][y].typ))
	    return "fountain";
	else if ((IS_ROOM(lev->typ) && !Is_earthlevel(&u.uz)) ||
		 IS_WALL(lev->typ) || IS_DOOR(lev->typ) || lev->typ == SDOOR)
	    return "floor";
	else
	    return "ground";
}

const char *
ceiling(x, y)
register int x, y;
{
	register struct rm *lev = &levl[x][y];
	const char *what;

	/* other room types will no longer exist when we're interested --
	 * see check_special_room()
	 */
	if (*in_rooms(x,y,VAULT))
	    what = "vault's ceiling";
	else if (*in_rooms(x,y,TEMPLE))
	    what = "temple's ceiling";
	else if (*in_rooms(x,y,SHOPBASE))
	    what = "shop's ceiling";
	else if (IS_AIR(lev->typ))
	    what = "sky";
	else if (Underwater)
	    what = "water's surface";
	else if ((IS_ROOM(lev->typ) && !Is_earthlevel(&u.uz)) ||
		 IS_WALL(lev->typ) || IS_DOOR(lev->typ) || lev->typ == SDOOR)
	    what = "ceiling";
	else
	    what = "rock above";

	return what;
}

struct engr *
engr_at(x, y)
xchar x, y;
{
	struct engr *ep = head_engr;

	while(ep) {
		if(x == ep->engr_x && y == ep->engr_y)
			return(ep);
		ep = ep->nxt_engr;
	}
	return((struct engr *) 0);
}

/* Decide whether a particular string is engraved at a specified
 * location; a case-insensitive substring match used.
 * Ignore headstones, in case the player names herself "Elbereth".
	Heh. Why not?
 */
int
sengr_at(s, x, y)
	const char *s;
	xchar x, y;
{
	register struct engr *ep = engr_at(x,y);

	return (ep && /*ep->engr_type != HEADSTONE &&*/
		ep->engr_time <= moves && strstri(ep->engr_txt, s) != 0);
}

int
ward_at(x, y)
	xchar x, y;
{
	register struct engr *ep = engr_at(x,y);

	if(!ep || ep->engr_time > moves) return 0;
	else if(ep->halu_ward) return ep->ward_id == CERULEAN_SIGN ? ELDER_SIGN : 0;
	else return ep->ward_id;
}

int
num_wards_at(x, y)
	xchar x, y;
{
	register struct engr *ep = engr_at(x,y);

	if(!ep || ep->engr_time > moves) return 0;
	else return ep->complete_wards;
}

#endif /* OVL0 */
#ifdef OVL2

void
u_wipe_engr(cnt)
register int cnt;
{
	if (can_reach_floor())
		wipe_engr_at(u.ux, u.uy, cnt);
}

#endif /* OVL2 */
#ifdef OVL1

void
wipe_engr_at(x,y,strt_cnt)
register xchar x,y,strt_cnt;
{
	register struct engr *ep = engr_at(x,y);
	xchar cnt;

	/* Headstones are indelible */
	if(ep && ep->engr_type != HEADSTONE){
	    if(ep->engr_txt[0] && (ep->engr_type != BURN || is_ice(x,y))) {
			if(ep->engr_type != DUST && ep->engr_type != ENGR_BLOOD) {
				cnt = rn2(1 + 50/(strt_cnt+1)) ? 0 : 1;
			}
			else cnt = strt_cnt;
			wipeout_text(ep->engr_txt, (int)cnt, 0);
			while(ep->engr_txt[0] == ' ')
				ep->engr_txt++;
		}
	    if(ep->ward_id && (ep->ward_type != BURN || is_ice(x,y))) {
			if(ep->ward_type != DUST && ep->ward_type != ENGR_BLOOD) {
				cnt = rn2(1 + 50/(strt_cnt+1)) ? 0 : 1;
			}
			else cnt = strt_cnt;
			wipeout_ward(ep, (int) cnt, 0);
		}
		if(!ep->engr_txt[0] && !ep->ward_id) del_engr_ward(ep);

	}
}

#endif /* OVL1 */
#ifdef OVL2

void
read_engr_at(x,y)
register int x,y;
{
	register struct engr *ep = engr_at(x,y);
	register int	sensed = 0;
	char buf[BUFSZ];
	char * word;
	
	/* Sensing an engraving does not require sight,
	 * nor does it necessarily imply comprehension (literacy).
	 */
	if(ep && ep->engr_txt[0]) {
	    switch(ep->engr_type) {
	    case DUST:
		if(!Blind) {
			sensed = 1;
			pline("%s is written here in the %s.", Something,
				is_ice(x,y) ? "frost" : "dust");
		}
		break;
	    case ENGRAVE:
	    case HEADSTONE:
		if (!Blind || can_reach_floor()) {
			sensed = 1;
			pline("%s is engraved here on the %s.",
				Something,
				surface(x,y));
		}
		break;
	    case BURN:
		if (!Blind || can_reach_floor()) {
			sensed = 1;
			pline("Some text has been %s into the %s here.",
				is_ice(x,y) ? "melted" : "burned",
				surface(x,y));
		}
		break;
	    case MARK:
		if(!Blind) {
			sensed = 1;
			pline("There's some graffiti on the %s here.",
				surface(x,y));
		}
		break;
	    case ENGR_BLOOD:
		/* "It's a message!  Scrawled in blood!"
		 * "What's it say?"
		 * "It says... `See you next Wednesday.'" -- Thriller
		 */
		if(!Blind) {
			sensed = 1;
			You("see a message scrawled in blood here.");
		}
		break;
	    default:
			impossible("%s is written in a very strange way.",
					Something);
			sensed = 1;
	    }
	    if (sensed) {
	    	char *et;
	    	unsigned maxelen = BUFSZ - sizeof("You feel the words: \"\". ");
			if(!Hallucination /*|| rn2(20)*/ ){
				if (strlen(ep->engr_txt) > maxelen) {
					(void) strncpy(buf,  ep->engr_txt, (int)maxelen);
					buf[maxelen] = '\0';
					et = buf;
				} else
					et = ep->engr_txt;
			}
			else{
				int haluDex = rn2(SIZE(haluMesg));
				if(strlen(haluMesg[haluDex]) > maxelen){
					(void) strncpy(buf,  haluMesg[haluDex], (int)maxelen);
					buf[maxelen] = '\0';
					et = buf;
				}
				else et = haluMesg[haluDex];
			}
			You("%s: \"%s\"",
				  (Blind) ? "feel the words" : "read",  et);
			if(flags.run > 1) nomul(0);
		}
	}
	if(ep && ep->ward_id){
		sensed = 0;
	    switch(ep->ward_type) {
	    case DUST:
		if(!Blind) {
			sensed = 1;
			word = is_ice(x,y) ? "drawn here in the frost." : 
					"drawn here in the dust.";
		}
		break;
	    case ENGRAVE:
	    case HEADSTONE:
		if (!Blind || can_reach_floor()) {
			sensed = 1;
			word = "carved here.";
		}
		break;
	    case BURN:
		if (!Blind || can_reach_floor()) {
			sensed = 1;
			word = is_ice(x,y) ? "melted into the ice here." : 
					"burned into the floor here.";
		
		}
		break;
	    case MARK:
		if(!Blind) {
			sensed = 1;
			word = "drawn in ink here.";
		}
		break;
	    case ENGR_BLOOD:
		/* "It's a message!  Scrawled in blood!"
		 * "What's it say?"
		 * "It says... `See you next Wednesday.'" -- Thriller
		 */
		if(!Blind) {
			sensed = 1;
			word = "drawn in blood here.";
		}
		break;
	    default:
			sensed = 1;
			word = "drawn here in a very strange way.";
	    }
	    if (sensed) {
		 if(!Hallucination){
		  if(!(ep->halu_ward)){
			int sum;
			if(ep->complete_wards){
				int wardhere = get_wardID(ep->ward_id);
				pline("There is %s %s", 
					wardText[ep->ward_id][ep->complete_wards-1],
					word);
				if( !(u.wardsknown & wardhere) ){
					You("have learned a new warding!");
					u.wardsknown |= wardhere;
				}
			}
			sum = ep->scuffed_wards + ep->degraded_wards + ep->partial_wards;
			if(sum){
				char wardbuf[BUFSZ];
				int wardbuflen = 0;
				wardbuf[0] = (char)0;
				if(ep->complete_wards) wardbuflen += sprintf(&wardbuf[wardbuflen], "There %s also", sum > 1 ? "are" : "is");
				else wardbuflen += sprintf(&wardbuf[wardbuflen], "There %s", sum > 1 ? "are" : "is");
				if(ep->scuffed_wards)
					wardbuflen += sprintf(&wardbuf[wardbuflen]," %d scuffed%s", ep->scuffed_wards, ep->degraded_wards && ep->partial_wards ? 
													"," : 
													ep->degraded_wards || ep->partial_wards ? 
													" and" : "");
				if(ep->degraded_wards)
					wardbuflen += sprintf(&wardbuf[wardbuflen]," %d degraded%s", ep->degraded_wards, ep->scuffed_wards && ep->partial_wards ? 
													", and" : 
													ep->partial_wards ? 
													" and" : "");
				if(ep->partial_wards)
					wardbuflen += sprintf(&wardbuf[wardbuflen]," %d fragmentary", ep->partial_wards);
				/*if(!(ep->complete_wards))*/ 
				wardbuflen += sprintf(&wardbuf[wardbuflen]," %s",
											wardNames[ep->ward_id][sum>1 ? 1 : 0]);
				pline("%d",wardbuflen);
				pline("%s %s", wardbuf, word);
			}
		  }
		  else{//note: assumes that there can only be ONE halucinatory ward scribed.
			if(ep->complete_wards) pline("There is %s %s", 
										haluWard[ep->ward_id],
										word);
			if(ep->scuffed_wards) pline("It is scuffed.");
			else if(ep->degraded_wards) pline("It is degraded.");
			else if(ep->partial_wards) pline("Although only fragments are left.");
		  }
		 }
		 else{
			pline("There is %s drawn here.", haluWard[rn2(SIZE(haluWard)-1)+1]);
		 }
		}
	}
}

#endif /* OVL2 */
#ifdef OVLB

void
make_engr_at(x,y,s,e_time,e_type)
int x,y;
const char *s;
long e_time;
xchar e_type;
{
	struct engr *ep;

	xchar exist_ward_type = 0;
	int exist_ward_id = 0;
	xchar exist_halu_ward = FALSE;
	xchar exist_complete_wards = 0;
	xchar exist_scuffed_wards = 0; 
	xchar exist_degraded_wards = 0;
	xchar exist_partial_wards = 0;

	if ((ep = engr_at(x,y)) != 0){
		exist_ward_type = ep->ward_type;
		exist_ward_id = ep->ward_id;
		exist_halu_ward = ep->halu_ward;
		exist_complete_wards = ep->complete_wards;
		exist_scuffed_wards = ep->scuffed_wards; 
		exist_degraded_wards = ep->degraded_wards;
		exist_partial_wards = ep->partial_wards;

	    del_engr_ward(ep);
	}
	ep = newengr(strlen(s) + 1);
	ep->nxt_engr = head_engr;
	head_engr = ep;
	ep->engr_x = x;
	ep->engr_y = y;
	ep->engr_txt = (char *)(ep + 1);
	Strcpy(ep->engr_txt, s);
	/* engraving Elbereth shows wisdom */
	if (!in_mklev && !strcmp(s, "Elbereth")){
		exercise(A_WIS, TRUE);
		u.uconduct.elbereth++;
	}
	ep->ward_type = exist_ward_type;
	ep->ward_id = exist_ward_id;
	ep->halu_ward = exist_halu_ward;
	ep->complete_wards = exist_complete_wards;
	ep->scuffed_wards = exist_scuffed_wards;
	ep->degraded_wards = exist_degraded_wards;
	ep->partial_wards = exist_partial_wards;
	ep->engr_time = e_time;
	ep->engr_type = e_type > 0 ? e_type : rnd(N_ENGRAVE-1);
	ep->engr_lth = strlen(s) + 1;
}

/* delete any engraving at location <x,y> */
void
del_engr_at(x, y)
int x, y;
{
	register struct engr *ep = engr_at(x, y);

	if (ep) del_engr(ep);
}

void
del_ward_at(x, y)
int x, y;
{
	register struct engr *ep = engr_at(x, y);

	if (ep) del_ward(ep);
}

void
del_engr_ward_at(x, y)
int x, y;
{
	register struct engr *ep = engr_at(x, y);

	if (ep) del_engr_ward(ep);
}

/*
 *	freehand - returns true if player has a free hand
 */
int
freehand()
{
	return(!uwep || !welded(uwep) ||
	   (!bimanual(uwep) && (!uarms || !uarms->cursed)));
/*	if ((uwep && bimanual(uwep)) ||
	    (uwep && uarms))
		return(0);
	else
		return(1);*/
}

static NEARDATA const char styluses[] =
	{ ALL_CLASSES, ALLOW_NONE, TOOL_CLASS, WEAPON_CLASS, WAND_CLASS,
	  GEM_CLASS, RING_CLASS, 0 };

	  
int
doengward()
{
    char c = 'n';
	if(u.wardsknown){
		c = yn_function("Do you want to scribe a warding sign?",
						ynqchars, 'q');
	}
	if(c == 'y') doward();
	else if(c=='n') doengrave();
	else return 0;
}

/* Mohs' Hardness Scale:
 *  1 - Talc		 6 - Orthoclase
 *  2 - Gypsum		 7 - Quartz
 *  3 - Calcite		 8 - Topaz
 *  4 - Fluorite	 9 - Corundum
 *  5 - Apatite		10 - Diamond
 *
 * Since granite is a igneous rock hardness ~ 7, anything >= 8 should
 * probably be able to scratch the rock.
 * Devaluation of less hard gems is not easily possible because obj struct
 * does not contain individual oc_cost currently. 7/91
 *
 * steel     -	5-8.5	(usu. weapon)
 * diamond    - 10			* jade	     -	5-6	 (nephrite)
 * ruby       -  9	(corundum)	* turquoise  -	5-6
 * sapphire   -  9	(corundum)	* opal	     -	5-6
 * topaz      -  8			* glass      - ~5.5
 * emerald    -  7.5-8	(beryl)		* dilithium  -	4-5??
 * aquamarine -  7.5-8	(beryl)		* iron	     -	4-5
 * garnet     -  7.25	(var. 6.5-8)	* fluorite   -	4
 * agate      -  7	(quartz)	* brass      -	3-4
 * amethyst   -  7	(quartz)	* gold	     -	2.5-3
 * jasper     -  7	(quartz)	* silver     -	2.5-3
 * onyx       -  7	(quartz)	* copper     -	2.5-3
 * moonstone  -  6	(orthoclase)	* amber      -	2-2.5
 */

/* return 1 if action took 1 (or more) moves, 0 if error or aborted */
int
doengrave()
{
	boolean dengr = FALSE;	/* TRUE if we wipe out the current engraving */
	boolean doblind = FALSE;/* TRUE if engraving blinds the player */
	boolean doknown = FALSE;/* TRUE if we identify the stylus */
	boolean eow = FALSE;	/* TRUE if we are overwriting oep */
	boolean jello = FALSE;	/* TRUE if we are engraving in slime */
	boolean ptext = TRUE;	/* TRUE if we must prompt for engrave text */
	boolean teleengr =FALSE;/* TRUE if we move the old engraving */
	boolean zapwand = FALSE;/* TRUE if we remove a wand charge */
	xchar type = DUST;	/* Type of engraving made */
	char buf[BUFSZ];	/* Buffer for final/poly engraving text */
	char ebuf[BUFSZ];	/* Buffer for initial engraving text */
	char qbuf[QBUFSZ];	/* Buffer for query text */
	char post_engr_text[BUFSZ]; /* Text displayed after engraving prompt */
	const char *everb;	/* Present tense of engraving type */
	const char *eloc;	/* Where the engraving is (ie dust/floor/...) */
	char *sp;		/* Place holder for space count of engr text */
	int len;		/* # of nonspace chars of new engraving text */
	int maxelen;		/* Max allowable length of engraving text */
	struct engr *oep = engr_at(u.ux,u.uy);
				/* The current engraving */
	struct obj *otmp;	/* Object selected with which to engrave */
	int randWard = 0; /* random ward */
	xchar randHalu = FALSE; /* whether or not the randWard should be read as a real or hallucinatory ward */
	char *writer;

	multi = 0;		/* moves consumed */
	nomovemsg = (char *)0;	/* occupation end message */

	buf[0] = (char)0;
	ebuf[0] = (char)0;
	post_engr_text[0] = (char)0;
	maxelen = BUFSZ - 1;
	if (is_demon(youmonst.data) || youmonst.data->mlet == S_VAMPIRE)
	    type = ENGR_BLOOD;

	/* Can the adventurer engrave at all? */

	if(u.uswallow) {
		if (is_animal(u.ustuck->data)) {
			pline("What would you write?  \"Jonah was here\"?");
			return(0);
		} else if (is_whirly(u.ustuck->data)) {
			You_cant("reach the %s.", surface(u.ux,u.uy));
			return(0);
		} else
			jello = TRUE;
	} else if (is_lava(u.ux, u.uy)) {
		You_cant("write on the lava!");
		return(0);
	} else if (is_pool(u.ux,u.uy) || IS_FOUNTAIN(levl[u.ux][u.uy].typ)) {
		You_cant("write on the water!");
		return(0);
	}
	if(Is_airlevel(&u.uz) || Is_waterlevel(&u.uz)/* in bubble */) {
		You_cant("write in thin air!");
		return(0);
	}
	if (cantwield(youmonst.data)) {
		You_cant("even hold anything!");
		return(0);
	}
	if (check_capacity((char *)0)) return (0);

	/* One may write with finger, or weapon, or wand, or..., or...
	 * Edited by GAN 10/20/86 so as not to change weapon wielded.
	 */

	otmp = getobj(styluses, "write with");
	if(!otmp) return(0);		/* otmp == zeroobj if fingers */

	if (otmp == &zeroobj) writer = makeplural(body_part(FINGER));
	else writer = xname(otmp);

	/* There's no reason you should be able to write with a wand
	 * while both your hands are tied up.
	 */
	if (!freehand() && otmp != uwep && !otmp->owornmask) {
		You("have no free %s to write with!", body_part(HAND));
		return(0);
	}

	if (jello) {
		You("tickle %s with your %s.", mon_nam(u.ustuck), writer);
		Your("message dissolves...");
		return(0);
	}
	if (otmp->oclass != WAND_CLASS && !can_reach_floor()) {
		You_cant("reach the %s!", surface(u.ux,u.uy));
		return(0);
	}
	if (IS_ALTAR(levl[u.ux][u.uy].typ)) {
		You("make a motion towards the altar with your %s.", writer);
		altar_wrath(u.ux, u.uy);
		return(0);
	}
	if (IS_GRAVE(levl[u.ux][u.uy].typ)) {
	    if (otmp == &zeroobj) { /* using only finger */
		You("would only make a small smudge on the %s.",
			surface(u.ux, u.uy));
		return(0);
	    } else if (!levl[u.ux][u.uy].disturbed) {
		You("disturb the undead!");
		levl[u.ux][u.uy].disturbed = 1;
		(void) makemon(&mons[PM_GHOUL], u.ux, u.uy, NO_MM_FLAGS);
		exercise(A_WIS, FALSE);
		return(1);
	    }
	}

	/* SPFX for items */

	switch (otmp->oclass) {
	    default:
	    case AMULET_CLASS:
	    case CHAIN_CLASS:
	    case POTION_CLASS:
	    case COIN_CLASS:
		break;

	    case RING_CLASS:
		/* "diamond" rings and others should work */
	    case GEM_CLASS:
		/* diamonds & other hard gems should work */
		if (objects[otmp->otyp].oc_tough) {
			type = ENGRAVE;
			break;
		}
		break;

	    case ARMOR_CLASS:
		if (is_boots(otmp)) {
			type = DUST;
			break;
		}
		/* fall through */
	    /* Objects too large to engrave with */
	    case BALL_CLASS:
	    case ROCK_CLASS:
/*	    case BED_CLASS:*/
		You_cant("engrave with such a large object!");
		ptext = FALSE;
		break;

	    /* Objects too silly to engrave with */
	    case FOOD_CLASS:
	    case SCROLL_CLASS:
	    case SPBOOK_CLASS:
		Your("%s would get %s.", xname(otmp),
			is_ice(u.ux,u.uy) ? "all frosty" : "too dirty");
		ptext = FALSE;
		break;

	    case RANDOM_CLASS:	/* This should mean fingers */
		break;

	    /* The charge is removed from the wand before prompting for
	     * the engraving text, because all kinds of setup decisions
	     * and pre-engraving messages are based upon knowing what type
	     * of engraving the wand is going to do.  Also, the player
	     * will have potentially seen "You wrest .." message, and
	     * therefore will know they are using a charge.
	     */
	    case WAND_CLASS:
		if (zappable(otmp)) {
		    check_unpaid(otmp);
		    zapwand = TRUE;
		    if (Levitation) ptext = FALSE;

		    switch (otmp->otyp) {
		    /* DUST wands */
		    default:
			break;

			/* NODIR wands */
		    case WAN_LIGHT:
		    case WAN_SECRET_DOOR_DETECTION:
		    case WAN_CREATE_MONSTER:
		    case WAN_WISHING:
		    case WAN_ENLIGHTENMENT:
			zapnodir(otmp);
			break;

			/* IMMEDIATE wands */
			/* If wand is "IMMEDIATE", remember to affect the
			 * previous engraving even if turning to dust.
			 */
		    case WAN_STRIKING:
			Strcpy(post_engr_text,
			"The wand unsuccessfully fights your attempt to write!"
			);
			break;
		    case WAN_SLOW_MONSTER:
			if (!Blind) {
			   Sprintf(post_engr_text,
				   "The bugs on the %s slow down!",
				   surface(u.ux, u.uy));
			}
			break;
		    case WAN_SPEED_MONSTER:
			if (!Blind) {
			   Sprintf(post_engr_text,
				   "The bugs on the %s speed up!",
				   surface(u.ux, u.uy));
			}
			break;
		    case WAN_POLYMORPH:
			if(oep)  {
			    if (oep->engr_txt[0]) {
					type = (xchar)0;	/* random */
					(void) random_engraving(buf);
			    }
				if(oep->ward_id){
					randWard = rn2(10) ? 1 : rn2(10) ? rn2(SIZE(haluWard)-1)+1 : 0;
					if(!randWard){
						randWard = rn2(NUMBER_OF_WARDS)+1;
						randHalu = FALSE;
					}
					else randHalu = TRUE;
				}
			    dengr = TRUE;
			}
			break;
		    case WAN_NOTHING:
		    case WAN_UNDEAD_TURNING:
		    case WAN_OPENING:
		    case WAN_LOCKING:
		    case WAN_PROBING:
			break;

			/* RAY wands */
		    case WAN_MAGIC_MISSILE:
			ptext = TRUE;
			if (!Blind) {
			   Sprintf(post_engr_text,
				   "The %s is riddled by bullet holes!",
				   surface(u.ux, u.uy));
			}
			break;

		    /* can't tell sleep from death - Eric Backus */
		    case WAN_SLEEP:
		    case WAN_DEATH:
			if (!Blind) {
			   Sprintf(post_engr_text,
				   "The bugs on the %s stop moving!",
				   surface(u.ux, u.uy));
			}
			break;

		    case WAN_COLD:
			if (!Blind)
			    Strcpy(post_engr_text,
				"A few ice cubes drop from the wand.");
			if(!oep || (oep->engr_type != BURN))
			    break;
		    case WAN_CANCELLATION:
		    case WAN_MAKE_INVISIBLE:
			if (oep && oep->engr_type != HEADSTONE) {
			    if (!Blind)
				pline_The("engraving on the %s vanishes!",
					surface(u.ux,u.uy));
			    dengr = TRUE;
			}
			break;
		    case WAN_TELEPORTATION:
			if (oep && oep->engr_type != HEADSTONE) {
			    if (!Blind)
				pline_The("engraving on the %s vanishes!",
					surface(u.ux,u.uy));
			    teleengr = TRUE;
			}
			break;

		    /* type = ENGRAVE wands */
		    case WAN_DIGGING:
			ptext = TRUE;
			type  = ENGRAVE;
			if(!objects[otmp->otyp].oc_name_known) {
			    if (flags.verbose)
				pline("This %s is a wand of digging!",
				xname(otmp));
			    doknown = TRUE;
			}
			if (!Blind)
			    Strcpy(post_engr_text,
				IS_GRAVE(levl[u.ux][u.uy].typ) ?
				"Chips fly out from the headstone." :
				is_ice(u.ux,u.uy) ?
				"Ice chips fly up from the ice surface!" :
				"Gravel flies up from the floor.");
			else
			    Strcpy(post_engr_text, "You hear drilling!");
			break;

		    /* type = BURN wands */
		    case WAN_FIRE:
			ptext = TRUE;
			type  = BURN;
			if(!objects[otmp->otyp].oc_name_known) {
			if (flags.verbose)
			    pline("This %s is a wand of fire!", xname(otmp));
			    doknown = TRUE;
			}
			Strcpy(post_engr_text,
				Blind ? "You feel the wand heat up." :
					"Flames fly from the wand.");
			break;
		    case WAN_LIGHTNING:
			ptext = TRUE;
			type  = BURN;
			if(!objects[otmp->otyp].oc_name_known) {
			    if (flags.verbose)
				pline("This %s is a wand of lightning!",
					xname(otmp));
			    doknown = TRUE;
			}
			if (!Blind) {
			    Strcpy(post_engr_text,
				    "Lightning arcs from the wand.");
			    doblind = TRUE;
			} else
			    Strcpy(post_engr_text, "You hear crackling!");
			break;

		    /* type = MARK wands */
		    /* type = ENGR_BLOOD wands */
		    }
		} else /* end if zappable */
		    if (!can_reach_floor()) {
			You_cant("reach the %s!", surface(u.ux,u.uy));
			return(0);
		    }
		break;

	    case WEAPON_CLASS:
		if (is_blade(otmp)) {
		    if ((int)otmp->spe > -3)
			type = ENGRAVE;
		    else
			Your("%s too dull for engraving.", aobjnam(otmp,"are"));
		}
		break;

	    case TOOL_CLASS:
		if(otmp == ublindf) {
		    pline(
		"That is a bit difficult to engrave with, don't you think?");
		    return(0);
		}
		switch (otmp->otyp)  {
		    case MAGIC_MARKER:
			if (otmp->spe <= 0)
			    Your("marker has dried out.");
			else
			    type = MARK;
			break;
		    case TOWEL:
			/* Can't really engrave with a towel */
			ptext = FALSE;
			if (oep)
			    if ((oep->engr_type == DUST ) ||
				(oep->engr_type == ENGR_BLOOD) ||
				(oep->engr_type == MARK )) {
				if (!Blind)
				    You("wipe out the message here.");
				else
				    Your("%s %s %s.", xname(otmp),
					 otense(otmp, "get"),
					 is_ice(u.ux,u.uy) ?
					 "frosty" : "dusty");
				dengr = TRUE;
			    } else
				Your("%s can't wipe out this engraving.",
				     xname(otmp));
			else{
			    Your("%s %s %s.", xname(otmp), otense(otmp, "get"),
				  is_ice(u.ux,u.uy) ? "frosty" : "dusty");
			}
			break;
		    default:
				if(spec_ability3(otmp, SPFX3_ENGRV)) type = ENGRAVE;
			break;
		}
		break;

	    case VENOM_CLASS:
#ifdef WIZARD
		if (wizard) {
		    pline("Writing a poison pen letter??");
		    break;
		}
#endif
	    case ILLOBJ_CLASS:
		impossible("You're engraving with an illegal object!");
		break;
	}

	if (IS_GRAVE(levl[u.ux][u.uy].typ)) {
	    if (type == ENGRAVE || type == 0)
		type = HEADSTONE;
	    else {
		/* ensures the "cannot wipe out" case */
		type = DUST;
		dengr = FALSE;
		teleengr = FALSE;
		buf[0] = (char)0;
	    }
	}

	/* End of implement setup */

	/* Identify stylus */
	if (doknown) {
	    makeknown(otmp->otyp);
	    more_experienced(0,10);
	}

	if (teleengr) {
	    rloc_engr(oep);
	    oep = (struct engr *)0;
	}

	if (dengr) {
	    del_engr_ward(oep);
	    oep = (struct engr *)0;
	}

	/* Something has changed the engraving here */
	if (*buf || randWard) {
		if(*buf){
			make_engr_at(u.ux, u.uy, buf, moves, type);
			oep = engr_at(u.ux,u.uy);
		}
		if(randWard){
			oep = engr_at(u.ux,u.uy);
			if(!oep){
				make_engr_at(u.ux, u.uy, "", moves, DUST);
				oep = engr_at(u.ux,u.uy);
			}
			oep->ward_id = randWard;
			oep->halu_ward = randHalu;
			oep->ward_type = rnd(N_ENGRAVE-1);
			oep->complete_wards = 1;
		}
	    if(!Blind){ 
			/*pline_The("engraving now reads: \"%s\".", buf);*/
			pline_The("engraving here has changed.");
			read_engr_at(u.ux,u.uy);
		}
	    ptext = FALSE;
	}

	if (zapwand && (otmp->spe < 0)) {
	    pline("%s %sturns to dust.",
		  The(xname(otmp)), Blind ? "" : "glows violently, then ");
	    if (!IS_GRAVE(levl[u.ux][u.uy].typ))
		You("are not going to get anywhere trying to write in the %s with your dust.",
		    is_ice(u.ux,u.uy) ? "frost" : "dust");
	    useup(otmp);
	    ptext = FALSE;
	}

	if (!ptext) {		/* Early exit for some implements. */
	    if (otmp->oclass == WAND_CLASS && !can_reach_floor())
		You_cant("reach the %s!", surface(u.ux,u.uy));
	    return(1);
	}

	/* Special effects should have deleted the current engraving (if
	 * possible) by now.
	 */

	if (oep) {
	    register char c = 'n';

	    /* Give player the choice to add to engraving. */

	    if (type == HEADSTONE) {
		/* no choice, only append */
		c = 'y';
	    } else if ( (type == oep->engr_type) && (!Blind ||
		 (oep->engr_type == BURN) || (oep->engr_type == ENGRAVE)) ) {
		c = yn_function("Do you want to add to the current engraving?",
				ynqchars, 'y');
		if (c == 'q') {
		    pline(Never_mind);
		    return(0);
		}
	    }

	    if (c == 'n' || Blind) {

		if( (oep->engr_type == DUST) || (oep->engr_type == ENGR_BLOOD) ||
		    (oep->engr_type == MARK) ) {
		    if (!Blind) {
			You("wipe out the message that was %s here.",
			    ((oep->engr_type == DUST)  ? "written in the dust" :
			    ((oep->engr_type == ENGR_BLOOD) ? "scrawled in blood"   :
							 "written")));
			del_engr(oep);
			oep = engr_at(u.ux,u.uy);
		    } else
		   /* Don't delete engr until after we *know* we're engraving */
			eow = TRUE;
		} else
		    if ( (type == DUST) || (type == MARK) || (type == ENGR_BLOOD) ) {
			You(
			 "cannot wipe out the message that is %s the %s here.",
			 oep->engr_type == BURN ?
			   (is_ice(u.ux,u.uy) ? "melted into" : "burned into") :
			   "engraved in", surface(u.ux,u.uy));
			return(1);
		    } else
			if ( (type != oep->engr_type) || (c == 'n') ) {
			    if (!Blind || can_reach_floor())
				You("will overwrite the current message.");
			    eow = TRUE;
			}
	    }
	}

	eloc = surface(u.ux,u.uy);
	switch(type){
	    default:
		everb = (oep && !eow ? "add to the weird writing on" :
				       "write strangely on");
		break;
	    case DUST:
		everb = (oep && !eow ? "add to the writing in" :
				       "write in");
		eloc = is_ice(u.ux,u.uy) ? "frost" : "dust";
		break;
	    case HEADSTONE:
		everb = (oep && !eow ? "add to the epitaph on" :
				       "engrave on");
		break;
	    case ENGRAVE:
		everb = (oep && !eow ? "add to the engraving in" :
				       "engrave in");
		break;
	    case BURN:
		everb = (oep && !eow ?
			( is_ice(u.ux,u.uy) ? "add to the text melted into" :
					      "add to the text burned into") :
			( is_ice(u.ux,u.uy) ? "melt into" : "burn into"));
		break;
	    case MARK:
		everb = (oep && !eow ? "add to the graffiti on" :
				       "scribble on");
		break;
	    case ENGR_BLOOD:
		everb = (oep && !eow ? "add to the scrawl on" :
				       "scrawl on");
		break;
	}

	/* Tell adventurer what is going on */
	if (otmp != &zeroobj)
	    You("%s the %s with %s.", everb, eloc, doname(otmp));
	else
	    You("%s the %s with your %s.", everb, eloc,
		makeplural(body_part(FINGER)));

	/* Prompt for engraving! */
	Sprintf(qbuf,"What do you want to %s the %s here?", everb, eloc);
	getlin(qbuf, ebuf);

	if(Hallucination && !rn2(20)){
		ebuf[0] = (char)0;
		Strcpy(ebuf, haluMesg[rn2(SIZE(haluMesg))]);		
	}
	/* Count the actual # of chars engraved not including spaces */
	len = strlen(ebuf);
	for (sp = ebuf; *sp; sp++) if (isspace(*sp)) len -= 1;

	if (len == 0 || index(ebuf, '\033')) {
	    if (zapwand) {
		if (!Blind)
		    pline("%s, then %s.",
			  Tobjnam(otmp, "glow"), otense(otmp, "fade"));
		return(1);
	    } else {
			pline(Never_mind);
			return(0);
	    }
	}

	/* A single `x' is the traditional signature of an illiterate person */
	if (len != 1 || (!index(ebuf, 'x') && !index(ebuf, 'X')))
	    u.uconduct.literate++;

	/* Mix up engraving if surface or state of mind is unsound.
	   Note: this won't add or remove any spaces. */
	for (sp = ebuf; *sp; sp++) {
	    if (isspace(*sp)) continue;
	    if (((type == DUST || type == ENGR_BLOOD) && !rn2(25)) ||
		    (Blind && !rn2(11)) || (Confusion && !rn2(7)) ||
		    (Stunned && !rn2(4)) || (Hallucination && !rn2(7)))
		*sp = ' ' + rnd(96 - 2);	/* ASCII '!' thru '~'
						   (excludes ' ' and DEL) */
	}

	/* Previous engraving is overwritten */
	if (eow) {
	    del_engr(oep);
	    oep = engr_at(u.ux,u.uy);
	}

	/* Figure out how long it took to engrave, and if player has
	 * engraved too much.
	 */
	switch(type){
	    default:
		multi = -(len/10);
		if (multi) nomovemsg = "You finish your weird engraving.";
		break;
	    case DUST:
		multi = -(len/10);
		if (multi) nomovemsg = "You finish writing in the dust.";
		break;
	    case HEADSTONE:
	    case ENGRAVE:
			multi = -(len/10);
			if ((otmp->oclass == WEAPON_CLASS || spec_ability3(otmp, SPFX3_ENGRV)) &&
				((otmp->otyp != ATHAME && !spec_ability3(otmp, SPFX3_ENGRV)) || otmp->cursed)) {
				multi = -len;
				maxelen = ((otmp->spe + 3) * 2) + 1;
				/* -2 = 3, -1 = 5, 0 = 7, +1 = 9, +2 = 11
				 * Note: this does not allow a +0 anything (except
				 *	 an athame) to engrave "Elbereth" all at once.
				 *	 However, you could now engrave "Elb", then
				 *	 "ere", then "th".
				 */
				Your("%s dull.", aobjnam(otmp, "get"));
				if (otmp->unpaid) {
				struct monst *shkp = shop_keeper(*u.ushops);
				if (shkp) {
					You("damage it, you pay for it!");
					bill_dummy_object(otmp);
				}
				}
				if (len > maxelen) {
				multi = -maxelen;
				otmp->spe = -3;
				} else if (len > 1)
				otmp->spe -= len >> 1;
				else otmp->spe -= 1; /* Prevent infinite engraving */
			} else if ( (otmp->oclass == RING_CLASS) ||
				 (otmp->oclass == GEM_CLASS) ) multi = -len;
			else if(otmp->oclass == WAND_CLASS){
				multi = -(len/10);
				maxelen = otmp->spe*10;
				if (multi < -1*otmp->spe){
					multi = -(otmp->spe);
					otmp->spe = 0;
					nomovemsg = is_ice(u.ux,u.uy) ?
					"You fail to finish blasting your message into the ice before running out of charges!":
					"You fail to finish blasting your message into the floor before running out of charges!";
				}
				else if (multi){
					otmp->spe += multi; /* multi is already negative*/
					nomovemsg = is_ice(u.ux,u.uy) ?
					"You finish blasting your message into the ice.":
					"You finish blasting your message into the floor.";
				}
			}
			if (multi && !nomovemsg) nomovemsg = "You finish engraving.";
		break;
	    case BURN:
			multi = -(len/10);
			maxelen = otmp->spe*10;
			if (multi < -1*otmp->spe){
				multi = -(otmp->spe);
				otmp->spe = 0;
				nomovemsg = is_ice(u.ux,u.uy) ?
				"You fail to finish melting your message into the ice before running out of charges!":
				"You fail to finish burning your message into the floor before running out of charges!";
			}
			else if (multi){
				otmp->spe += multi; /* multi is already negative*/
				nomovemsg = is_ice(u.ux,u.uy) ?
				"You finish melting your message into the ice.":
				"You finish burning your message into the floor.";
			}
		break;
	    case MARK:
		multi = -(len/10);
		if ((otmp->oclass == TOOL_CLASS) &&
		    (otmp->otyp == MAGIC_MARKER)) {
		    maxelen = (otmp->spe) * 2; /* one charge / 2 letters */
		    if (len > maxelen) {
			Your("marker dries out.");
			otmp->spe = 0;
			multi = -(maxelen/10);
		    } else
			if (len > 1) otmp->spe -= len >> 1;
			else otmp->spe -= 1; /* Prevent infinite grafitti */
		}
		if (multi) nomovemsg = "You finish defacing the dungeon.";
		break;
	    case ENGR_BLOOD:
		multi = -(len/10);
		if (multi) nomovemsg = "You finish scrawling.";
		break;
	}

	/* Chop engraving down to size if necessary */
	if (len > maxelen) {
	    for (sp = ebuf; (maxelen && *sp); sp++)
		if (!isspace(*sp)) maxelen--;
	    if (!maxelen && *sp) {
		*sp = (char)0;
		if (multi) nomovemsg = "You cannot write any more.";
		You("only are able to write \"%s\"", ebuf);
	    }
	}

	/* Add to existing engraving */
	if (oep) Strcpy(buf, oep->engr_txt);

	(void) strncat(buf, ebuf, (BUFSZ - (int)strlen(buf) - 1));

	make_engr_at(u.ux, u.uy, buf, (moves - multi), type);

	if (post_engr_text[0]) pline(post_engr_text);

	if (doblind && !resists_blnd(&youmonst)) {
	    You("are blinded by the flash!");
	    make_blinded((long)rnd(50),FALSE);
	    if (!Blind) Your(vision_clears);
	}

	return(1);
}

/* return 1 if action took 1 (or more) moves, 0 if error or aborted */
int
doward()
{
	boolean dengr = FALSE;	/* TRUE if we wipe out the current ward */
	boolean doblind = FALSE;/* TRUE if warding blinds the player */
	boolean doknown = FALSE;/* TRUE if we identify the stylus */
	boolean eow = FALSE;	/* TRUE if we are overwriting oep */
	boolean jello = FALSE;	/* TRUE if we are engraving in slime */
	boolean ptext = TRUE;	/* TRUE if we must prompt for engrave text */
	boolean teleengr =FALSE;/* TRUE if we move the old engraving */
	boolean zapwand = FALSE;/* TRUE if we remove a wand charge */
	xchar type = DUST;	/* Type of ward made */
	char buf[BUFSZ];	/* Buffer for final/poly engraving text */
	char ebuf[BUFSZ];	/* Buffer for initial engraving text */
	char qbuf[QBUFSZ];	/* Buffer for query text */
	char post_engr_text[BUFSZ]; /* Text displayed after engraving prompt */
	const char *everb;	/* Present tense of engraving type */
	const char *eloc;	/* Where the engraving is (ie dust/floor/...) */
	char *sp;		/* Place holder for space count of engr text */
	int ward = 0;	/* ID number of the ward to be engraved */
	int len = 0;		/* # of nonspace chars of new engraving text */
	int maxelen;		/* Max allowable length of engraving text */
	struct engr *oep = engr_at(u.ux,u.uy);
				/* The current engraving */
	struct obj *otmp;	/* Object selected with which to engrave */
	int randWard = 0; /* random ward */
	xchar randHalu = FALSE; /* whether or not the randWard should be read as a real or hallucinatory ward */
	char *writer;

	multi = 0;		/* moves consumed */
	nomovemsg = (char *)0;	/* occupation end message */

	buf[0] = (char)0;
	ebuf[0] = (char)0;
	post_engr_text[0] = (char)0;
	maxelen = BUFSZ - 1;
	if (is_demon(youmonst.data) || youmonst.data->mlet == S_VAMPIRE)
	    type = ENGR_BLOOD;

	/* Can the adventurer engrave at all? */

	if(u.uswallow) {
		if (is_animal(u.ustuck->data)) {
			pline("What would you do, write \"Jonah was here\"?");
			return(0);
		} else if (is_whirly(u.ustuck->data)) {
			You_cant("reach the %s.", surface(u.ux,u.uy));
			return(0);
		} else
			jello = TRUE;
	} else if (is_lava(u.ux, u.uy)) {
		You_cant("draw on the lava!");
		return(0);
	} else if (is_pool(u.ux,u.uy) || IS_FOUNTAIN(levl[u.ux][u.uy].typ)) {
		You_cant("draw on the water!");
		return(0);
	}
	if(Is_airlevel(&u.uz) || Is_waterlevel(&u.uz)/* in bubble */) {
		You_cant("draw in thin air!");
		return(0);
	}
	if (cantwield(youmonst.data)) {
		You_cant("even hold anything!");
		return(0);
	}
	if (check_capacity((char *)0)) return (0);

	/* One may draw with finger, or weapon, or wand, or..., or...
	 * Edited by GAN 10/20/86 so as not to change weapon wielded.
	 */

	otmp = getobj(styluses, "draw with");
	if(!otmp) return(0);		/* otmp == zeroobj if fingers */

	if (otmp == &zeroobj) writer = makeplural(body_part(FINGER));
	else writer = xname(otmp);

	/* There's no reason you should be able to draw with a wand
	 * while both your hands are tied up.
	 */
	if (!freehand() && otmp != uwep && !otmp->owornmask) {
		You("have no free %s to draw with!", body_part(HAND));
		return(0);
	}

	if (jello) {
		You("tickle %s with your %s.", mon_nam(u.ustuck), writer);
		Your("message dissolves...");
		return(0);
	}
	if (otmp->oclass != WAND_CLASS && !can_reach_floor()) {
		You_cant("reach the %s!", surface(u.ux,u.uy));
		return(0);
	}
	if (IS_ALTAR(levl[u.ux][u.uy].typ)) {
		You("make a motion towards the altar with your %s.", writer);
		altar_wrath(u.ux, u.uy);
		return(0);
	}
	if (IS_GRAVE(levl[u.ux][u.uy].typ)) {
		You("don't have room to draw a ward on the %s.",
			surface(u.ux, u.uy));
			return(0);
	}

	/* SPFX for items */

	switch (otmp->oclass) {
	    default:
	    case AMULET_CLASS:
	    case CHAIN_CLASS:
	    case POTION_CLASS:
	    case COIN_CLASS:
		break;

	    case RING_CLASS:
		/* "diamond" rings and others should work */
	    case GEM_CLASS:
		/* diamonds & other hard gems should work */
		if (objects[otmp->otyp].oc_tough) {
			type = ENGRAVE;
			break;
		}
		break;

	    case ARMOR_CLASS:
		if (is_boots(otmp)) {
			type = DUST;
			break;
		}
		/* fall through */
	    /* Objects too large to draw with */
	    case BALL_CLASS:
	    case ROCK_CLASS:
/*	    case BED_CLASS:*/
		You_cant("draw with such a large object!");
		ptext = FALSE;
		break;

	    /* Objects too silly to draw with */
	    case FOOD_CLASS:
	    case SCROLL_CLASS:
	    case SPBOOK_CLASS:
		Your("%s would get %s.", xname(otmp),
			is_ice(u.ux,u.uy) ? "all frosty" : "too dirty");
		ptext = FALSE;
		break;

	    case RANDOM_CLASS:	/* This should mean fingers */
		break;

	    /* The charge is removed from the wand before prompting for
	     * the engraving text, because all kinds of setup decisions
	     * and pre-engraving messages are based upon knowing what type
	     * of engraving the wand is going to do.  Also, the player
	     * will have potentially seen "You wrest .." message, and
	     * therefore will know they are using a charge.
	     */
	    case WAND_CLASS:
		if (zappable(otmp)) {
		    check_unpaid(otmp);
		    zapwand = TRUE;
		    if (Levitation) ptext = FALSE;

		    switch (otmp->otyp) {
		    /* DUST wands */
		    default:
			break;

			/* NODIR wands */
		    case WAN_LIGHT:
		    case WAN_SECRET_DOOR_DETECTION:
		    case WAN_CREATE_MONSTER:
		    case WAN_WISHING:
		    case WAN_ENLIGHTENMENT:
			zapnodir(otmp);
			break;

			/* IMMEDIATE wands */
			/* If wand is "IMMEDIATE", remember to affect the
			 * previous engraving even if turning to dust.
			 */
		    case WAN_STRIKING:
			Strcpy(post_engr_text,
			"The wand unsuccessfully fights your attempt to draw!"
			);
			break;
		    case WAN_SLOW_MONSTER:
			if (!Blind) {
			   Sprintf(post_engr_text,
				   "The bugs on the %s slow down!",
				   surface(u.ux, u.uy));
			}
			break;
		    case WAN_SPEED_MONSTER:
			if (!Blind) {
			   Sprintf(post_engr_text,
				   "The bugs on the %s speed up!",
				   surface(u.ux, u.uy));
			}
			break;
		    case WAN_POLYMORPH:
			if(oep)  {
			    if (oep->engr_txt[0]) {
					type = (xchar)0;	/* random */
					(void) random_engraving(buf);
			    }
				if(oep->ward_id){
					randWard = rn2(10) ? 1 : rn2(10) ? rn2(SIZE(haluWard)-1)+1 : 0;
					if(!randWard){
						randWard = rn2(NUMBER_OF_WARDS)+1;
						randHalu = FALSE;
					}
					else randHalu = TRUE;
				}
			    dengr = TRUE;
			}
			break;
		    case WAN_NOTHING:
		    case WAN_UNDEAD_TURNING:
		    case WAN_OPENING:
		    case WAN_LOCKING:
		    case WAN_PROBING:
			break;

			/* RAY wands */
		    case WAN_MAGIC_MISSILE:
			ptext = TRUE;
			if (!Blind) {
			   Sprintf(post_engr_text,
				   "The %s is riddled by bullet holes!",
				   surface(u.ux, u.uy));
			}
			break;

		    /* can't tell sleep from death - Eric Backus */
		    case WAN_SLEEP:
		    case WAN_DEATH:
			if (!Blind) {
			   Sprintf(post_engr_text,
				   "The bugs on the %s stop moving!",
				   surface(u.ux, u.uy));
			}
			break;

		    case WAN_COLD:
			if (!Blind)
			    Strcpy(post_engr_text,
				"A few ice cubes drop from the wand.");
			if(!oep || (oep->engr_type != BURN))
			    break;
		    case WAN_CANCELLATION:
		    case WAN_MAKE_INVISIBLE:
			if (oep && oep->engr_type != HEADSTONE) {
			    if (!Blind)
				pline_The("engraving on the %s vanishes!",
					surface(u.ux,u.uy));
			    dengr = TRUE;
			}
			break;
		    case WAN_TELEPORTATION:
			if (oep && oep->engr_type != HEADSTONE) {
			    if (!Blind)
				pline_The("engraving on the %s vanishes!",
					surface(u.ux,u.uy));
			    teleengr = TRUE;
			}
			break;

		    /* type = ENGRAVE wands */
		    case WAN_DIGGING:
			ptext = TRUE;
			type  = ENGRAVE;
			if(!objects[otmp->otyp].oc_name_known) {
			    if (flags.verbose)
				pline("This %s is a wand of digging!",
				xname(otmp));
			    doknown = TRUE;
			}
			if (!Blind)
			    Strcpy(post_engr_text,
				IS_GRAVE(levl[u.ux][u.uy].typ) ?
				"Chips fly out from the headstone." :
				is_ice(u.ux,u.uy) ?
				"Ice chips fly up from the ice surface!" :
				"Gravel flies up from the floor.");
			else
			    Strcpy(post_engr_text, "You hear drilling!");
			break;

		    /* type = BURN wands */
		    case WAN_FIRE:
			ptext = TRUE;
			type  = BURN;
			if(!objects[otmp->otyp].oc_name_known) {
			if (flags.verbose)
			    pline("This %s is a wand of fire!", xname(otmp));
			    doknown = TRUE;
			}
			Strcpy(post_engr_text,
				Blind ? "You feel the wand heat up." :
					"Flames fly from the wand.");
			break;
		    case WAN_LIGHTNING:
			ptext = TRUE;
			type  = BURN;
			if(!objects[otmp->otyp].oc_name_known) {
			    if (flags.verbose)
				pline("This %s is a wand of lightning!",
					xname(otmp));
			    doknown = TRUE;
			}
			if (!Blind) {
			    Strcpy(post_engr_text,
				    "Lightning arcs from the wand.");
			    doblind = TRUE;
			} else
			    Strcpy(post_engr_text, "You hear crackling!");
			break;

		    /* type = MARK wands */
		    /* type = ENGR_BLOOD wands */
		    }
		} else /* end if zappable */
		    if (!can_reach_floor()) {
			You_cant("reach the %s!", surface(u.ux,u.uy));
			return(0);
		    }
		break;

	    case WEAPON_CLASS:
		if (is_blade(otmp)) {
		    if ((int)otmp->spe > -3)
			type = ENGRAVE;
		    else
			Your("%s too dull for drawing.", aobjnam(otmp,"are"));
		}
		break;

	    case TOOL_CLASS:
		if(otmp == ublindf) {
		    pline(
		"That is a bit difficult to draw with, don't you think?");
		    return(0);
		}
		switch (otmp->otyp)  {
		    case MAGIC_MARKER:
			if (otmp->spe <= 0)
			    Your("marker has dried out.");
			else
			    type = MARK;
			break;
		    case TOWEL:
			/* Can't really engrave with a towel */
			ptext = FALSE;
			if (oep)
			    if ((oep->engr_type == DUST ) ||
				(oep->engr_type == ENGR_BLOOD) ||
				(oep->engr_type == MARK )) {
				if (!Blind)
				    You("wipe out the message here.");
				else
				    Your("%s %s %s.", xname(otmp),
					 otense(otmp, "get"),
					 is_ice(u.ux,u.uy) ?
					 "frosty" : "dusty");
				dengr = TRUE;
			    } else
				Your("%s can't wipe out this engraving.",
				     xname(otmp));
			else{
			    Your("%s %s %s.", xname(otmp), otense(otmp, "get"),
				  is_ice(u.ux,u.uy) ? "frosty" : "dusty");
			}
			break;
		    default:
				if(spec_ability3(otmp, SPFX3_ENGRV)) type = ENGRAVE;
			break;
		}
		break;

	    case VENOM_CLASS:
#ifdef WIZARD
		if (wizard) {
		    pline("Writing a poison pen letter??");
		    break;
		}
#endif
	    case ILLOBJ_CLASS:
		impossible("You're drawing with an illegal object!");
		break;
	}

	if (IS_GRAVE(levl[u.ux][u.uy].typ)) {
	    if (type == ENGRAVE || type == 0)
		type = HEADSTONE;
	    else {
		/* ensures the "cannot wipe out" case */
		type = DUST;
		dengr = FALSE;
		teleengr = FALSE;
		buf[0] = (char)0;
	    }
	}

	/* End of implement setup */

	/* Identify stylus */
	if (doknown) {
	    makeknown(otmp->otyp);
	    more_experienced(0,10);
	}

	if (teleengr) {
	    rloc_engr(oep);
	    oep = (struct engr *)0;
	}

	if (dengr) {
	    del_engr_ward(oep);
	    oep = (struct engr *)0;
	}

	/* Something has changed the engraving here */
	if (*buf || randWard) {
		if(*buf){
			make_engr_at(u.ux, u.uy, buf, moves, type);
			oep = engr_at(u.ux,u.uy);
		}
		if(randWard){
			oep = engr_at(u.ux,u.uy);
			if(!oep){
				make_engr_at(u.ux, u.uy, "", moves, DUST);
				oep = engr_at(u.ux,u.uy);
			}
			oep->ward_id = randWard;
			oep->halu_ward = randHalu;
			oep->ward_type = rnd(N_ENGRAVE-1);
			oep->complete_wards = 1;
		}
	    if(!Blind){ 
			/*pline_The("engraving now reads: \"%s\".", buf);*/
			pline_The("engraving here has changed.");
			read_engr_at(u.ux,u.uy);
		}
	    ptext = FALSE;
	}

	if (zapwand && (otmp->spe < 0)) {
	    pline("%s %sturns to dust.",
		  The(xname(otmp)), Blind ? "" : "glows violently, then ");
	    if (!IS_GRAVE(levl[u.ux][u.uy].typ))
		You("are not going to get anywhere trying to draw in the %s with your dust.",
		    is_ice(u.ux,u.uy) ? "frost" : "dust");
	    useup(otmp);
	    ptext = FALSE;
	}

	if (!ptext) {		/* Early exit for some implements. */
	    if (otmp->oclass == WAND_CLASS && !can_reach_floor())
		You_cant("reach the %s!", surface(u.ux,u.uy));
	    return(1);
	}

	/* Special effects should have deleted the current engraving (if
	 * possible) by now.
	 */
	if (oep && oep->ward_id) {
		char c = 'n';

	    /* Give player the choice to add to ward. */

	    if (type == HEADSTONE) {
		/* headstones are not big enough for wards */
			pline("This headstone is not big enough for drawing one");
		    return(0);
	    } else if ( (type == oep->ward_type) && !Hallucination && (!Blind ||
		 (oep->ward_type == BURN) || (oep->ward_type == ENGRAVE)) ) {
			c = yn_function("Do you want to reinforce the existing ward?",
					ynqchars, 'y');
			if (c == 'q') {
				pline(Never_mind);
				return(0);
			}
	    }
		
	    if (c == 'n' || Blind) {

			if( (oep->ward_type == DUST) || (oep->ward_type == ENGR_BLOOD) ||
				(oep->ward_type == MARK) ) {
				if (!Blind) {
				You("wipe out the ward that was %s here.",
					((oep->ward_type == DUST)  ? "drawn in the dust" :
					((oep->ward_type == ENGR_BLOOD) ? "painted in blood"   :
								 "drawn")));
				del_ward(oep);
				oep = engr_at(u.ux,u.uy);
				} else
			   /* Don't delete engr until after we *know* we're engraving */
				eow = TRUE;
			} else{
				if ( (type == DUST) || (type == MARK) || (type == ENGR_BLOOD) ) {
				You(
				 "cannot wipe out the ward that is %s the %s here.",
				 oep->ward_type == BURN ?
				   (is_ice(u.ux,u.uy) ? "melted into" : "burned into") :
				   "engraved in", surface(u.ux,u.uy));
				return(1);
				} else
				if ( (type != oep->ward_type) || (c == 'n') ) {
					if (!Blind || can_reach_floor())
					You("will replace the current ward.");
					eow = TRUE;
				}
			}
	    }
		else if(c == 'y'){
			int newWards = wardsAdded[oep->ward_id][oep->complete_wards];
			int increment;
			if(oep->complete_wards >= 7 || newWards < 1){
				pline("The warding sign can be reinforced no further!");
				return 0;
			}
			/*pline("%d to be added, %d there already", newWards, oep->complete_wards);*/
			boolean continue_loop = TRUE;
			ward = oep->ward_id;
			len = wardStrokes[ward][oep->complete_wards];
			increment = len/newWards;
			while(newWards && continue_loop){
				if(oep->scuffed_wards){
					len -= (int)(increment*.9);
					newWards--;
					oep->scuffed_wards--;
				}
				else if(oep->degraded_wards){
					len -= (int) (increment*.75);
					newWards--;
					oep->degraded_wards--;
				}
				else if(oep->partial_wards){
					len -= (int) (increment*.75);
					newWards--;
					oep->degraded_wards--;
				}
				else continue_loop = FALSE;
			}
		}
	}

	eloc = surface(u.ux,u.uy);
	switch(type){
	    default:
		everb = (oep && !eow ? "add to the weird drawing on" :
				       "draw strangely on");
		break;
	    case DUST:
		everb = (oep && !eow ? "add to the drawing in" :
				       "draw in");
		eloc = is_ice(u.ux,u.uy) ? "frost" : "dust";
		break;
	    case HEADSTONE:
		everb = (oep && !eow ? "add to the drawing on" :
				       "draw on");
		break;
	    case ENGRAVE:
		everb = (oep && !eow ? "add to the drawing in" :
				       "draw in");
		break;
	    case BURN:
		everb = (oep && !eow ?
			( is_ice(u.ux,u.uy) ? "add to the drawing melted into" :
					      "add to the drawing burned into") :
			( is_ice(u.ux,u.uy) ? "melt into" : "burn into"));
		break;
	    case MARK:
		everb = (oep && !eow ? "add to the graffiti on" :
				       "draw on");
		break;
	    case ENGR_BLOOD:
		everb = (oep && !eow ? "add to the drawing on" :
				       "draw on");
		break;
	}

	/* Tell adventurer what is going on */
	if (otmp != &zeroobj)
	    You("%s the %s with %s.", everb, eloc, doname(otmp));
	else
	    You("%s the %s with your %s.", everb, eloc,
		makeplural(body_part(FINGER)));

	/* Prompt for engraving! */
	if(!len){
		ward = pick_ward();
		len = wardStrokes[ward][0];
	}
	if (ward == 0 || index(ebuf, '\033')) {
	    if (zapwand) {
			if (!Blind)
				pline("%s, then %s.",
				  Tobjnam(otmp, "glow"), otense(otmp, "fade"));
			return(1);
	    } else {
			pline(Never_mind);
			return(0);
	    }
	}
	u.uconduct.wardless++;
	if (eow) {
		del_ward(oep);
		oep = engr_at(u.ux,u.uy);
	}
	switch(type){
	    default:
			multi = -(len/10);
			if (multi) nomovemsg = "You finish your weird drawing.";
		break;
	    case DUST:
			multi = -(len/10);
			if (multi) nomovemsg = "You finish drawing in the dust.";
		break;
	    case HEADSTONE:
	    case ENGRAVE:
			multi = -(len/10);
			if ((otmp->oclass == WEAPON_CLASS || spec_ability3(otmp, SPFX3_ENGRV)) &&
				((otmp->otyp != ATHAME && !spec_ability3(otmp, SPFX3_ENGRV)) || otmp->cursed)) {
				multi = -len;
				maxelen = ((otmp->spe + 3) * 2) + 1;
				/* -2 = 3, -1 = 5, 0 = 7, +1 = 9, +2 = 11
				 */
				Your("%s dull.", aobjnam(otmp, "get"));
				if (otmp->unpaid) {
					struct monst *shkp = shop_keeper(*u.ushops);
					if (shkp) {
						You("damage it, you pay for it!");
						bill_dummy_object(otmp);
					}
				}
				if (len > maxelen) {
					multi = -maxelen;
					otmp->spe = -3;
				} else if (len > 1)
					otmp->spe -= len >> 1;
				else otmp->spe -= 1; /* Prevent infinite engraving */
			} else if ( (otmp->oclass == RING_CLASS) ||
				 (otmp->oclass == GEM_CLASS) ) multi = -len;
			else if(otmp->oclass == WAND_CLASS){
				multi = -(len/10);
				maxelen = otmp->spe*10;
				if(multi < -1*otmp->spe){
					multi = -(otmp->spe);
					otmp->spe = 0;
					nomovemsg = is_ice(u.ux,u.uy) ?
					"You fail to finish blasting your drawing into the ice before running out of charges!":
					"You fail to finish blasting your drawing into the floor before running out of charges!";
				}
				else if (multi){
					otmp->spe += multi; /* multi is already negative*/
					nomovemsg = is_ice(u.ux,u.uy) ?
					"You finish blasting your drawing into the ice.":
					"You finish blasting your drawing into the floor.";
				}
			}
			if (multi && !nomovemsg) nomovemsg = "You finish drawing.";
		break;
	    case BURN:
			multi = -(len/10);
			maxelen = otmp->spe*10;
			if(multi < -1*otmp->spe){
				multi = -(otmp->spe);
				otmp->spe = 0;
				nomovemsg = is_ice(u.ux,u.uy) ?
				"You fail to finish melting your drawing into the ice before running out of charges!":
				"You fail to finish burning your drawing into the floor before running out of charges!";
			}
			else if (multi){
				otmp->spe += multi; /* multi is already negative*/
				nomovemsg = is_ice(u.ux,u.uy) ?
				"You finish melting your drawing into the ice.":
				"You finish burning your drawing into the floor.";
			}
		break;
	    case MARK:
			multi = -(len/10);
			if ((otmp->oclass == TOOL_CLASS) &&
				(otmp->otyp == MAGIC_MARKER)) {
				maxelen = (otmp->spe) * 2; /* one charge / 2 letters */
				if (len > maxelen) {
				Your("marker dries out.");
				otmp->spe = 0;
				multi = -(maxelen/10);
				} else
				if (len > 1) otmp->spe -= len >> 1;
				else otmp->spe -= 1; /* Prevent infinite grafitti */
			}
			if (multi) nomovemsg = "You finish defacing the dungeon.";
		break;
	    case ENGR_BLOOD:
			multi = -(len/10);
			if (multi) nomovemsg = "You finish scrawling.";
		break;
	}

	/* Chop engraving down to size if necessary */
	/* BETA: engraving fails if too long */
	if (len > maxelen) {
	    for (sp = ebuf; (maxelen && *sp); sp++)
		if (!isspace(*sp)) maxelen--;
	    if (!maxelen && *sp) {
		*sp = (char)0;
		if (multi) nomovemsg = "Unfortunatly, you can't complete the symbol.";
		You("can't complete the symbol.", ebuf);
	    }
	}
	else if (oep && oep->ward_id){
		oep->complete_wards += wardsAdded[oep->ward_id][oep->complete_wards];
	}
	else if(oep){
		if(!Hallucination || !rn2(4)){
			oep->ward_id = ward;
			oep->ward_type = type;
			oep->complete_wards = wardsAdded[oep->ward_id][0];
		}
		else{
			oep->ward_id = rn2(4) ? 1 : rn2(100) ? rn2(SIZE(haluWard)-1)+1 : 0;
			if(!oep->ward_id){
				oep->ward_id = rn2(NUMBER_OF_WARDS)+1;
				oep->halu_ward = FALSE;
			}
			else oep->halu_ward = TRUE;
			oep->ward_type = type;
			oep->complete_wards = 1;
		}
	}
	else{
		make_engr_at(u.ux, u.uy, "", (moves - multi), DUST); /* absense of text =  dust */
		oep = engr_at(u.ux,u.uy);
		if(!Hallucination){
			oep->ward_id = ward;
			oep->ward_type = type;
			oep->complete_wards = wardsAdded[oep->ward_id][0];
		}
		else{
			oep->ward_id = rn2(4) ? 1 : rn2(100) ? rn2(SIZE(haluWard)-1)+1 : 0;
			if(!oep->ward_id){
				oep->ward_id = rn2(NUMBER_OF_WARDS)+1;
				oep->halu_ward = FALSE;
			}
			else oep->halu_ward = TRUE;
			oep->ward_type = type;
			oep->complete_wards = 1;
		}
	}

	if (post_engr_text[0]) pline(post_engr_text);

	if (doblind && !resists_blnd(&youmonst)) {
	    You("are blinded by the flash!");
	    make_blinded((long)rnd(50),FALSE);
	    if (!Blind) Your(vision_clears);
	}

	return(1);
}

int
pick_ward()
{
	winid tmpwin;
	int n, how;
	char buf[BUFSZ];
	char incntlet = 'a';
	menu_item *selected;
	anything any;

	tmpwin = create_nhwindow(NHW_MENU);
	start_menu(tmpwin);
	any.a_void = 0;		/* zero out all bits */

	Sprintf(buf, "Known Wards");
	add_menu(tmpwin, NO_GLYPH, &any, 0, 0, ATR_BOLD, buf, MENU_UNSELECTED);
	if(u.wardsknown & WARD_HEPTAGRAM){
		Sprintf(buf, "Heptagram");
		any.a_int = HEPTAGRAM;	/* must be non-zero */
		add_menu(tmpwin, NO_GLYPH, &any,
			incntlet, 0, ATR_NONE, buf,
			MENU_UNSELECTED);
		incntlet = (incntlet != 'z') ? (incntlet+1) : 'A';
	}
	if(u.wardsknown & WARD_GORGONEION){
		Sprintf(buf, "Gorgoneion");
		any.a_int = GORGONEION;	/* must be non-zero */
		add_menu(tmpwin, NO_GLYPH, &any,
			incntlet, 0, ATR_NONE, buf,
			MENU_UNSELECTED);
		incntlet = (incntlet != 'z') ? (incntlet+1) : 'A';
	}
	if(u.wardsknown & WARD_ACHERON){
		Sprintf(buf, "Circle of Acheron");
		any.a_int = CIRCLE_OF_ACHERON;	/* must be non-zero */
		add_menu(tmpwin, NO_GLYPH, &any,
			incntlet, 0, ATR_NONE, buf,
			MENU_UNSELECTED);
		incntlet = (incntlet != 'z') ? (incntlet+1) : 'A';
	}
	if(u.wardsknown & WARD_PENTAGRAM){
		Sprintf(buf, "Pentagram");
		any.a_int = PENTAGRAM;	/* must be non-zero */
		add_menu(tmpwin, NO_GLYPH, &any,
			incntlet, 0, ATR_NONE, buf,
			MENU_UNSELECTED);
		incntlet = (incntlet != 'z') ? (incntlet+1) : 'A';
	}
	if(u.wardsknown & WARD_HEXAGRAM){
		Sprintf(buf, "Hexagram");
		any.a_int = HEXAGRAM;	/* must be non-zero */
		add_menu(tmpwin, NO_GLYPH, &any,
			incntlet, 0, ATR_NONE, buf,
			MENU_UNSELECTED);
		incntlet = (incntlet != 'z') ? (incntlet+1) : 'A';
	}
	if(u.wardsknown & WARD_HAMSA){
		Sprintf(buf, "Hamsa");
		any.a_int = HAMSA;	/* must be non-zero */
		add_menu(tmpwin, NO_GLYPH, &any,
			incntlet, 0, ATR_NONE, buf,
			MENU_UNSELECTED);
		incntlet = (incntlet != 'z') ? (incntlet+1) : 'A';
	}
	if(u.wardsknown & WARD_ELDER_SIGN){
		Sprintf(buf, "Elder Sign");
		any.a_int = ELDER_SIGN;	/* must be non-zero */
		add_menu(tmpwin, NO_GLYPH, &any,
			incntlet, 0, ATR_NONE, buf,
			MENU_UNSELECTED);
		incntlet = (incntlet != 'z') ? (incntlet+1) : 'A';
	}
	if(u.wardsknown & WARD_EYE){
		Sprintf(buf, "Elder Elemental Eye");
		any.a_int = ELDER_ELEMENTAL_EYE;	/* must be non-zero */
		add_menu(tmpwin, NO_GLYPH, &any,
			incntlet, 0, ATR_NONE, buf,
			MENU_UNSELECTED);
		incntlet = (incntlet != 'z') ? (incntlet+1) : 'A';
	}
	if(u.wardsknown & WARD_QUEEN){
		Sprintf(buf, "Sign of the Scion Queen Mother");
		any.a_int = SIGN_OF_THE_SCION_QUEEN;	/* must be non-zero */
		add_menu(tmpwin, NO_GLYPH, &any,
			incntlet, 0, ATR_NONE, buf,
			MENU_UNSELECTED);
		incntlet = (incntlet != 'z') ? (incntlet+1) : 'A';
	}
	if(u.wardsknown & WARD_CAT_LORD){
		Sprintf(buf, "Cartouche of the Cat Lord");
		any.a_int = CARTOUCHE_OF_THE_CAT_LORD;	/* must be non-zero */
		add_menu(tmpwin, NO_GLYPH, &any,
			incntlet, 0, ATR_NONE, buf,
			MENU_UNSELECTED);
		incntlet = (incntlet != 'z') ? (incntlet+1) : 'A';
	}
	if(u.wardsknown & WARD_GARUDA){
		Sprintf(buf, "The Wings of Garuda");
		any.a_int = WINGS_OF_GARUDA;	/* must be non-zero */
		add_menu(tmpwin, NO_GLYPH, &any,
			incntlet, 0, ATR_NONE, buf,
			MENU_UNSELECTED);
		incntlet = (incntlet != 'z') ? (incntlet+1) : 'A';
	}
	end_menu(tmpwin, "Choose ward:");

	how = PICK_ONE;
	n = select_menu(tmpwin, how, &selected);
	destroy_nhwindow(tmpwin);
	return (n < SELECT_STUDY && n > 0) ? selected[0].item.a_int : 0;
}

int
decode_wardID(wardID)
int wardID;
{
	switch(wardID){
		case WARD_HEPTAGRAM: return HEPTAGRAM;
		break;
		case WARD_GORGONEION: return GORGONEION;
		break;
		case WARD_ACHERON: return CIRCLE_OF_ACHERON;
		break;
		case WARD_PENTAGRAM: return PENTAGRAM;
		break;
		case WARD_HEXAGRAM: return HEXAGRAM;
		break;
		case WARD_HAMSA: return HAMSA;
		break;
		case WARD_ELDER_SIGN: return ELDER_SIGN;
		break;
		case WARD_EYE: return ELDER_ELEMENTAL_EYE;
		break;
		case WARD_QUEEN: return SIGN_OF_THE_SCION_QUEEN;
		break;
		case WARD_CAT_LORD: return CARTOUCHE_OF_THE_CAT_LORD;
		break;
		case WARD_GARUDA: return WINGS_OF_GARUDA;
		break;
		default:
		return 0;
		break;
	}
}

int
get_wardID(floorID)
int floorID;
{
	switch(floorID){
		case HEPTAGRAM: return WARD_HEPTAGRAM;
		break;
		case GORGONEION: return WARD_GORGONEION;
		break;
		case CIRCLE_OF_ACHERON: return WARD_ACHERON;
		break;
		case PENTAGRAM: return WARD_PENTAGRAM;
		break;
		case HEXAGRAM: return WARD_HEXAGRAM;
		break;
		case HAMSA: return WARD_HAMSA;
		break;
		case ELDER_SIGN: return WARD_ELDER_SIGN;
		break;
		case ELDER_ELEMENTAL_EYE: return WARD_EYE;
		break;
		case SIGN_OF_THE_SCION_QUEEN: return WARD_QUEEN;
		break;
		case CARTOUCHE_OF_THE_CAT_LORD: return WARD_CAT_LORD;
		break;
		case WINGS_OF_GARUDA: return WARD_GARUDA;
		break;
		default:
		return 0;
		break;
	}
}

void
save_engravings(fd, mode)
int fd, mode;
{
	register struct engr *ep = head_engr;
	register struct engr *ep2;
	unsigned no_more_engr = 0;

	while (ep) {
	    ep2 = ep->nxt_engr;
	    if (((ep->engr_lth && ep->engr_txt[0]) || ep->ward_id) && perform_bwrite(mode)) {
			bwrite(fd, (genericptr_t)&(ep->engr_lth), sizeof(ep->engr_lth));
			bwrite(fd, (genericptr_t)ep, sizeof(struct engr) + ep->engr_lth);
	    }
	    if (release_data(mode))
			dealloc_engr(ep);
	    ep = ep2;
	}
	if (perform_bwrite(mode))
	    bwrite(fd, (genericptr_t)&no_more_engr, sizeof no_more_engr);
	if (release_data(mode))
	    head_engr = 0;
}

void
rest_engravings(fd)
int fd;
{
	register struct engr *ep;
	unsigned lth;

	head_engr = 0;
	while(1) {
		mread(fd, (genericptr_t) &lth, sizeof(unsigned));
		if(lth == 0) return;
		ep = newengr(lth);
		mread(fd, (genericptr_t) ep, sizeof(struct engr) + lth);
		ep->nxt_engr = head_engr;
		head_engr = ep;
		ep->engr_txt = (char *) (ep + 1);	/* Andreas Bormann */
		/* mark as finished for bones levels -- no problem for
		 * normal levels as the player must have finished engraving
		 * to be able to move again */
		ep->engr_time = moves;
	}
}

void
del_engr_ward(ep)
register struct engr *ep;
{
	if (ep == head_engr) {
		head_engr = ep->nxt_engr;
	} else {
		register struct engr *ept;

		for (ept = head_engr; ept; ept = ept->nxt_engr)
		    if (ept->nxt_engr == ep) {
			ept->nxt_engr = ep->nxt_engr;
			break;
		    }
		if (!ept) {
		    impossible("Error in del_engr_ward?");
		    return;
		}
	}
	dealloc_engr(ep);
}

void
del_engr(ep)
register struct engr *ep;
{
	if(!ep->ward_id) del_engr_ward(ep);
	else make_engr_at(ep->engr_x,ep->engr_y,"",moves,DUST);
}

void
del_ward(ep)
register struct engr *ep;
{
	if(!ep->engr_txt[0]) del_engr_ward(ep);
	else{
		ep->ward_type = 0;
		ep->ward_id = 0;
		ep->halu_ward = FALSE;
		ep->complete_wards = 0;
		ep->scuffed_wards = 0;
		ep->degraded_wards = 0;
		ep->partial_wards = 0;
	}
}

/* randomly relocate an engraving */
void
rloc_engr(ep)
struct engr *ep;
{
	int tx, ty, tryct = 200;

	do  {
	    if (--tryct < 0) return;
	    tx = rn1(COLNO-3,2);
	    ty = rn2(ROWNO);
	} while (engr_at(tx, ty) ||
		!goodpos(tx, ty, (struct monst *)0, 0));

	ep->engr_x = tx;
	ep->engr_y = ty;
}


/* Epitaphs for random headstones */
static const char *epitaphs[] = {
	"Rest in peace",
	"R.I.P.",
	"Rest In Pieces",
	"Note -- there are NO valuable items in this grave",
	"1994-1995. The Longest-Lived Hacker Ever",
	"The Grave of the Unknown Hacker",
	"We weren't sure who this was, but we buried him here anyway",
	"Sparky -- he was a very good dog",
	"Beware of Electric Third Rail",
	"Made in Taiwan",
	"Og friend. Og good dude. Og died. Og now food",
	"Beetlejuice Beetlejuice Beetlejuice",
	"Look out below!",
	"Please don't dig me up. I'm perfectly happy down here. -- Resident",
	"Postman, please note forwarding address: Gehennom, Asmodeus's Fortress, fifth lemure on the left",
	"Mary had a little lamb/Its fleece was white as snow/When Mary was in trouble/The lamb was first to go",
	"Be careful, or this could happen to you!",
	"Soon you'll join this fellow in hell! -- the Wizard of Yendor",
	"Caution! This grave contains toxic waste",
	"Sum quod eris",
	"Here lies an Atheist, all dressed up and no place to go",
	"Here lies Ezekiel, age 102.  The good die young.",
	"Here lies my wife: Here let her lie! Now she's at rest and so am I.",
	"Here lies Johnny Yeast. Pardon me for not rising.",
	"He always lied while on the earth and now he's lying in it",
	"I made an ash of myself",
	"Soon ripe. Soon rotten. Soon gone. But not forgotten.",
	"Here lies the body of Jonathan Blake. Stepped on the gas instead of the brake.",
	"Go away!",
        "Alas fair Death, 'twas missed in life - some peace and quiet from my wife",
        "Applaud, my friends, the comedy is finished.",
        "At last... a nice long sleep.",
        "Audi Partem Alteram",
        "Basil, assaulted by bears",
        "Burninated",
        "Confusion will be my epitaph",
        "Do not open until Christmas",
        "Don't be daft, they couldn't hit an elephant at this dist-",
        "Don't forget to stop and smell the roses",
        "Don't let this happen to you!",
        "Dulce et decorum est pro patria mori",
        "Et in Arcadia ego",
        "Fatty and skinny went to bed.  Fatty rolled over and skinny was dead.  Skinny Smith 1983-2000.",
        "Finally I am becoming stupider no more",
        "Follow me to hell",
        "...for famous men have the whole earth as their memorial",
        "Game over, man.  Game over.",
        "Go away!  I'm trying to take a nap in here!  Bloody adventurers...",
        "Gone fishin'",
        "Good night, sweet prince: And flights of angels sing thee to thy rest!",
        "Go Team Ant!",
        "He farmed his way here",
        "Here lies a programmer.  Killed by a fatal error.",
        "Here lies Bob - decided to try an acid blob",
        "Here lies Dudley, killed by another %&#@#& newt.",
        "Here lies Gregg, choked on an egg",
        "Here lies Lies. It's True",
        "Here lies The Lady's maid, died of a Vorpal Blade",
        "Here lies the left foot of Jack, killed by a land mine.  Let us know if you find any more of him",
        "He waited too long",
        "I'd rather be sailing",
        "If a man's deeds do not outlive him, of what value is a mark in stone?",
        "I'm gonna make it!",
        "I took both pills!",
        "I will survive!",
        "Killed by a black dragon -- This grave is empty",
        "Let me out of here!",
        "Lookin' good, Medusa.",
        "Mrs. Smith, choked on an apple.  She left behind grieving husband, daughter, and granddaughter.",
        "Nobody believed her when she said her feet were killing her",
        "No!  I don't want to see my damn conduct!",
        "One corpse, sans head",
        "On the whole, I'd rather be in Minetown",
        "On vacation",
        "Oops.",
        "Out to Lunch",
        "SOLD",
        "Someone set us up the bomb!",
        "Take my stuff, I don't need it anymore",
        "Taking a year dead for tax reasons",
        "The reports of my demise are completely accurate",
        "(This space for sale)",
        "This was actually just a pit, but since there was a corpse, we filled it",
        "This way to the crypt",
        "Tu quoque, Brute?",
        "VACANCY",
        "Welcome!",
        "Wish you were here!",
        "Yea, it got me too",
        "You should see the other guy",
        "...and they made me engrave my own headstone too!",
        "...but the blood has stopped pumping and I am left to decay...",
        "<Expletive Deleted>",
        "A masochist is never satisfied.",
        "Ach, 'twas a wee monster in the loch",
        "Adapt.  Enjoy.  Survive.",
        "Adventure, hah!  Excitement, hah!",
        "After all, what are friends for...",
        "After this, nothing will shock me",
        "After three days, fish and guests stink",
        "Age and treachery will always overcome youth and skill",
        "Ageing is not so bad.  The real killer is when you stop.",
        "Ain't I a stinker?",
        "Algernon",
        "All else failed...",
        "All hail RNG",
        "All right, we'll call it a draw!",
        "All's well that end well",
        "Alone at last!",
        "Always attack a floating eye from behind!",
        "Am I having fun yet?",
        "And I can still crawl, I'm not dead yet!",
        "And all I wanted was a free lunch",
        "And all of the signs were right there on your face",
        "And don't give me that innocent look either!",
        "And everyone died.  Boo hoo hoo.",
        "And here I go again...",
        "And nobody cares until somebody famous dies...",
        "And so it ends?",
        "And so... it begins.",
        "And sometimes the bear eats you.",
        "And then 'e nailed me 'ead to the floor!",
        "And they said it couldn't be done!",
        "And what do I look like?  The living?",
        "And yes, it was ALL his fault!",
        "And you said it was pretty here...",
        "Another lost soul",
        "Any day above ground is a good day!",
        "Any more of this and I'll die of a stroke before I'm 30.",
        "Anybody seen my head?",
        "Anyone for deathmatch?",
        "Anything for a change.",
        "Anything that kills you makes you ... well, dead",
        "Anything worth doing is worth overdoing.",
        "Are unicorns supposedly peaceful if you're a virgin?  Hah!",
        "Are we all being disintegrated, or is it just me?",
        "At least I'm good at something",
        "Attempted suicide",
        "Auribus teneo lupum",
        "Be prepared",
        "Beauty survives",
        "Been Here. Now Gone. Had a Good Time.",
        "Been through Hell, eh?  What did you bring me?",
        "Beg your pardon, didn't recognize you, I've changed a lot.",
        "Being dead builds character",
        "Beloved daughter, a treasure, buried here.",
        "Best friends come and go...  Mine just die.",
        "Better be dead than a fat slave",
        "Better luck next time",
        "Beware of Discordians bearing answers",
        "Beware the ...",
        "Bloody Hell...",
        "Bloody barbarians!",
        "Blown upward out of sight: He sought the leak by candlelight",
        "Brains... Brains... Fresh human brains...",
        "Buried the cat.  Took an hour.  Damn thing kept fighting.",
        "But I disarmed the trap!",
        "CONNECT 1964 - NO CARRIER 1994",
        "Call me if you need my phone number!",
        "Can YOU fly?",
        "Can you believe that thing is STILL moving?",
        "Can you come up with some better ending for this?",
        "Can you feel anything when I do this?",
        "Can you give me mouth to mouth, you just took my breath away.",
        "Can't I just have a LITTLE peril?",
        "Can't eat, can't sleep, had to bury the husband here.",
        "Can't you hit me?!",
        "Chaos, panic and disorder.  My work here is done.",
        "Check enclosed.",
        "Check this out!  It's my brain!",
        "Chivalry is only reasonably dead",
        "Coffin for sale.  Lifetime guarantee.",
        "Come Monday, I'll be all right.",
        "Come and see the violence inherent in the system",
        "Come back here!  I'll bite your bloody knees off!",
        "Commodore Business Machines, Inc.   Died for our sins.",
        "Complain to one who can help you",
        "Confess my sins to god?  Which one?",
        "Confusion will be my epitaph",
        "Cooties?  Ain't no cooties on me!",
        "Could somebody get this noose off me?",
        "Could you check again?  My name MUST be there.",
        "Could you please take a breath mint?",
        "Couldn't I be sedated for this?",
        "Courage is looking at your setbacks with serenity",
        "Cover me, I'm going in!",
        "Crash course in brain surgery",
        "Cross my fingers for me.",
        "Curse god and die",
        "Cut to fit",
        "De'Ath",
        "Dead Again?  Pardon me for not getting it right the first time!",
        "Dead and loving every moment!",
        "Dear wife of mine. Died of a broken heart, after I took it out of her.",
        "Don't tread on me!",
        "Dragon? What dragon?",
        "Drawn and quartered",
        "Either I'm dead or my watch has stopped.",
        "Eliza -- Was I really alive, or did I just think I was?",
        "Elvis",
        "Enter not into the path of the wicked",
        "Eris?  I don't need Eris",
        "Eternal Damnation, Come and stay a long while!",
        "Even The Dead pay taxes (and they aren't Grateful).",
        "Even a tomb stone will say good things when you're down!",
        "Ever notice that live is evil backwards?",
        "Every day is starting to look like Monday",
        "Every day, in every way, I am getting better and better.",
        "Every survival kit should include a sense of humor",
        "Evil I did dwell;  lewd did I live",
        "Ex post fucto",
        "Excellent day to have a rotten day.",
        "Excuse me for not standing up.",
        "Experience isn't everything. First, You've got to survive",
        "First shalt thou pull out the Holy Pin",
        "For a Breath, I Tarry...",
        "For recreational use only.",
        "For sale: One soul, slightly used. Asking for 3 wishes.",
        "For some moments in life, there are no words.",
        "Forget Disney World, I'm going to Hell!",
        "Forget about the dog, Beware of my wife.",
        "Funeral - Real fun.",
        "Gawd, it's depressing in here, isn't it?",
        "Genuine Exploding Gravestone.  (c)Acme Gravestones Inc.",
        "Get back here!  I'm not finished yet...",
        "Go ahead, I dare you to!",
        "Go ahead, it's either you or him.",
        "Goldilocks -- This casket is just right",
        "Gone But Not Forgotten",
        "Gone Underground For Good",
        "Gone away owin' more than he could pay.",
        "Gone, but not forgiven",
        "Got a life. Didn't know what to do with it.",
        "Grave?  But I was cremated!",
        "Greetings from Hell - Wish you were here.",
        "HELP! It's dark in here... Oh, my eyes are closed - sorry",
        "Ha! I NEVER pay income tax!",
        "Have you come to raise the dead?",
        "Having a good time can be deadly.",
        "Having a great time. Where am I exactly??",
        "He died of the flux.",
        "He died today... May we rest in peace!",
        "He got the upside, I got the downside.",
        "He lost his face when he was beheaded.",
        "He missed me first.",
        "He's not dead, he just smells that way.",
        "Help! I've fallen and I can't get up!",
        "Help, I can't wake up!",
        "Here lies Pinocchio",
        "Here lies the body of John Round. Lost at sea and never found.",
        "Here there be dragons",
        "Hey, I didn't write this stuff!",
        "Hold my calls",
        "Home Sweet Hell",
        "Humpty Dumpty, a Bad Egg.  He was pushed off the wall.",
        "I KNEW this would happen if I lived long enough.",
        "I TOLD you I was sick!",
        "I ain't broke but I am badly bent.",
        "I ain't old. I'm chronologically advantaged.",
        "I am NOT a vampire. I just like to bite..nibble, really!",
        "I am here. Wish you were fine.",
        "I am not dead yet, but watch for further reports.",
        "I believe them bones are me.",
        "I broke his brain.",
        "I can feel it.  My mind.  It's going.  I can feel it.",
        "I can't go to Hell. They're afraid I'm gonna take over!",
        "I can't go to hell, they don't want me.",
        "I didn't believe in reincarnation the last time, either.",
        "I didn't mean it when I said 'Bite me'",
        "I died laughing",
        "I disbelieved in reincarnation in my last life, too.",
        "I hacked myself to death",
        "I have all the time in the world",
        "I knew I'd find a use for this gravestone!",
        "I know my mind. And it's around here someplace.",
        "I lied!  I'll never be alright!",
        "I like it better in the dark.",
        "I like to be here when I can.",
        "I may rise but I refuse to shine.",
        "I never get any either.",
        "I said hit HIM with the fireball, not me!",
        "I told you I would never say goodbye.",
        "I used to be amusing. Now I'm just disgusting.",
        "I used up all my sick days, so now I'm calling in dead.",
        "I was killed by <illegible scrawl>",
        "I was somebody. Who, is no business of yours.",
        "I will not go quietly.",
        "I'd give you a piece of my mind... but I can't find it.",
        "I'd rather be breathing",
        "I'll be back!",
        "I'll be mellow when I'm dead. For now, let's PARTY!",
        "I'm doing this only for tax purposes.",
        "I'm not afraid of Death!  What's he gonna do? Kill me?",
        "I'm not getting enough money, so I'm not going to engrave anything useful here.",
        "I'm not saying anything.",
        "I'm weeth stupeed --->",
        "If you thought you had problems...",
        "Ignorance kills daily.",
        "Ignore me... I'm just here for my looks!",
        "Ilene Toofar -- Fell off a cliff",
        "Is that all?",
        "Is there life before Death?",
        "Is this a joke, or a grave matter?",
        "It happens sometimes. People just explode.",
        "It must be Thursday. I never could get the hang of Thursdays.",
        "It wasn't a fair fight",
        "It wasn't so easy.",
        "It's Loot, Pillage and THEN Burn...",
        "Just doing my job here",
        "Killed by diarrhea of mouth and constipation of brain.",
        "Let her RIP",
        "Let it be; I am dead.",
        "Let's play Hide the Corpse",
        "Life is NOT a dream",
        "Madge Ination -- It wasn't all in my head",
        "Meet me in Heaven",
        "Move on, there's nothing to see here.",
        "Mr. Flintstone -- Yabba-dabba-done",
        "My heart is not in this",
        "No one ever died from it",
        "No, you want room 12A, next door.",
        "Nope.  No trap on that chest.  I swear.",
        "Not again!",
        "Not every soil can bear all things",
        "Now I have a life",
        "Now I lay thee down to sleep... wanna join me?",
        "OK, here is a question: Where ARE your tanlines?",
        "Obesa Cantavit",
        "Oh! An untimely death.",
        "Oh, by the way, how was my funeral?",
        "Oh, honey..I missed you! She said, and fired again.",
        "Ok, so the light does go off. Now let me out of here.",
        "One stone brain",
        "Ooh! Somebody STOP me!",
        "Oops!",
        "Out for the night.  Leave a message.",
        "Ow!  Do that again!",
        "Pardon my dust.",
        "Part of me still works.",
        "Please, not in front of those orcs!",
        "Prepare to meet me in Heaven",
        "R2D2 -- Rest, Tin Piece",
        "Relax.  Nothing ever happens on the first level.",
        "Res omnia mea culpa est",
        "Rest In Pieces",
        "Rest, rest, perturbed spirit.",
        "Rip Torn",
        "She always said her feet were killing her but nobody believed her.",
        "She died of a chest cold.",
        "So let it be written, so let it be done!",
        "So then I says, How do I know you're the real angel of death?",
        "Some patients insist on dying.",
        "Some people have it dead easy, don't they?",
        "Some things are better left buried.",
        "Sure, trust me, I'm a lawyer...",
        "Thank God I wore my corset, because I think my sides have split.",
        "That is all",
        "The Gods DO have a sense of humor: I'm living proof!",
        "The frog's dead. He Kermitted suicide.",
        "This dungeon is a pushover",
        "This elevator doesn't go to Heaven",
        "This gravestone is shareware. To register, please send me 10 zorkmids",
        "This gravestone provided by The Yendorian Grave Services Inc.",
        "This is not an important part of my life.",
        "This one's on me.",
        "This side up",
        "Tim Burr -- Smashed by a tree",
        "Tone it down a bit, I'm trying to get some rest here.",
        "Virtually Alive",
        "We Will Meet Again.",
        "Weep not, he is at rest",
        "Welcome to Dante's.  What level please?",
        "Well, at least they listened to my sermon...",
        "Went to be an angel.",
        "What are you doing over there?",
        "What are you smiling at?",
        "What can you say, Death's got appeal...!",
        "What health care?",
        "What pit?",
        "When the gods want to punish you, they answer your prayers.",
        "Where e'er you be let your wind go free. Keeping it in was the death of me!",
        "Where's my refund?",
        "Will let you know for sure in a day or two...",
        "Wizards are wimps",
        "Worms at work, do not disturb!",
        "Would you mind moving a bit?  I'm short of breath down here.",
        "Would you quit being evil over my shoulder?",
        "Ya really had me going baby, but now I'm gone.",
        "Yes Dear, just a few more minutes...",
        "You said it wasn't poisonous!",
        "You set my heart aflame. You gave me heartburn."
};

/* Create a headstone at the given location.
 * The caller is responsible for newsym(x, y).
 */
void
make_grave(x, y, str)
int x, y;
const char *str;
{
	/* Can we put a grave here? */
	if ((levl[x][y].typ != ROOM && levl[x][y].typ != GRAVE) || t_at(x,y)) return;

	/* Make the grave */
	levl[x][y].typ = GRAVE;

	/* Engrave the headstone */
	if (!str) str = epitaphs[rn2(SIZE(epitaphs))];
	del_engr_ward_at(x, y);
	make_engr_at(x, y, str, 0L, HEADSTONE);
	return;
}


#endif /* OVLB */

/*engrave.c*/
