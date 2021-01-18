/* See LICENSE file for copyright and license details. */

#define TERMINAL "st"

/* appearance */
static const unsigned int borderpx  = 4;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=12", "JoyPixels:pixelsize=12:antialias=true:autohint=true" };
static const char dmenufont[]       = "monospace:size=12";
static const char col_gray1[]       = "#282828";
static const char col_gray2[]       = "#928374";
static const char col_gray3[]       = "#a89984";
static const char col_gray4[]       = "#ebdbb2";
static const char col_cyan[]        = "#689d6a";
static const char col_green[]       = "#98971a";
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*                  fg         bg         border   */
	[SchemeNorm]    = { col_gray3, col_gray1, col_green },
	[SchemeSel]     = { col_gray4, col_cyan,  col_cyan  },
	[SchemeStatus]  = { col_gray4, col_gray1,  "#000000"  }, // Statusbar right {text,background,not used but cannot be empty}
};
static const unsigned int alphas[][3]      = {
	/*                  fg      bg        border     */
	[SchemeNorm]    = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]     = { OPAQUE, baralpha, borderalpha },
	[SchemeStatus]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { "", "", "", "", "", "", "", "", "" };
static const char *tagsalt[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title         tags mask     iscentered   isfloating   monitor */
	{ "Gimp",     NULL,       NULL,         0,            0,           1,           -1 },
	{ "firefox",  NULL,       NULL,         1 << 1,       0,           0,            1 },
	{ "qutebrowser", NULL,    NULL,         1 << 1,       0,           0,            1 },
	{ "Pcmanfm",  NULL,       NULL,         1 << 2,       0,           0,           -1 },
	{ "minecraft-launcher", NULL, NULL,     1 << 3,       0,           0,           -1 },
	{ "Skype",    NULL,       NULL,         1 << 4,       0,           0,            1 },
	{ "Discord",  NULL,       NULL,         1 << 4,       0,           0,            1 },
	{ NULL,       NULL,       "neomutt",    1 << 6,       1,           0,           -1 },
	{ "Spotify",  NULL,       NULL,         1 << 7,       0,           0,            1 },
	{ NULL,       NULL,       "pulsemixer", 0,            1,           1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      comboview,      {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      combotag,       {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
	{ MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \
	/* { MOD, XK_q,     ACTION##stack, {.i = 0 } }, \
	{ MOD, XK_a,     ACTION##stack, {.i = 1 } }, \
	{ MOD, XK_z,     ACTION##stack, {.i = 2 } }, \
	{ MOD, XK_x,     ACTION##stack, {.i = -1 } }, */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
#define TERMCMD(cmd) { .v = (const char*[]){ TERMINAL, "-e", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_recency", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *mailcmd[]  = { TERMINAL, "-e", "neomutt", NULL };
static const char *firefox[]  = { "firefox", NULL };
static const char *firefoxpriv[] = { "firefox", "--private-window", NULL };
static const char *qutebrowser[] = { "qutebrowser", NULL };
static const char *wallchanger[] = { "sxiv", "/home/jasper/.local/share/wallpapers", "-t", "-r", NULL };
static const char *rndbg[]    = { "setbg", "/usr/share/backgrounds/wallpapers/", NULL };
static const char *fileman[]  = { "pcmanfm", NULL };

#include <X11/XF86keysym.h>

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          SHCMD("samedir") },
	{ MODKEY|Mod1Mask,              XK_Return, spawn,          SHCMD("samedir -b") },
	{ MODKEY|ControlMask,           XK_Return, spawn,          {.v = qutebrowser } },
	{ MODKEY|ShiftMask,             XK_BackSpace, quit,        {0} },
	{ MODKEY,                       XK_space,  zoom,           {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },

	{ MODKEY,                       XK_F2,     spawn,          {.v = firefox } },
	{ MODKEY|ShiftMask,             XK_F2,     spawn,          {.v = firefoxpriv } },
	{ MODKEY,                       XK_F3,     spawn,          {.v = fileman }},

	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_w,      spawn,          {.v = wallchanger } },
	{ MODKEY|ShiftMask,             XK_w,      spawn,          {.v = rndbg } },
	/* { MODKEY,                       XK_e,      spawn,          SHCMD("") }, */
	/* { MODKEY,                       XK_r,      spawn,          SHCMD("") }, */
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_y,      spawn,          SHCMD("$BROWSER https://youtube.com") },
	{ MODKEY,                       XK_u,      spawn,          SHCMD("dmenuunicode") },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_i,      incnmaster,     {.i = -1 } },
	/* { MODKEY,                       XK_o,      spawn,          SHCMD("") }, */
	{ MODKEY,                       XK_p,      spawn,          SHCMD("passmenu2 -l 5") },
	{ MODKEY|ShiftMask,             XK_p,      spawn,          SHCMD("passmenu2 --type -l 5") },
	/* { MODKEY,                       XK_a,      spawn,          SHCMD("") }, */
	{ MODKEY|ShiftMask,             XK_s,      togglealwaysontop, {0} },
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_g,      togglealttag,   {0} },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	/* { MODKEY,                       XK_z,      spawn,          SHCMD("") }, */
	/* { MODKEY,                       XK_x,      spawn,          SHCMD("") }, */
	{ MODKEY,                       XK_c,      setcentered,    {0} },
	/* { MODKEY,                       XK_v,      spawn,          SHCMD("") }, */
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_n,      spawn,          {.v = mailcmd } },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_m,      spawn,          TERMCMD("pulsemixer") },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },

	{ MODKEY|Mod1Mask,              XK_j,      moveresize,     {.v = "0x 25y 0w 0h" } },
	{ MODKEY|Mod1Mask,              XK_k,      moveresize,     {.v = "0x -25y 0w 0h" } },
	{ MODKEY|Mod1Mask,              XK_l,      moveresize,     {.v = "25x 0y 0w 0h" } },
	{ MODKEY|Mod1Mask,              XK_h,      moveresize,     {.v = "-25x 0y 0w 0h" } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_j,      moveresize,     {.v = "0x 0y 0w 25h" } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_k,      moveresize,     {.v = "0x 0y 0w -25h" } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_l,      moveresize,     {.v = "0x 0y 25w 0h" } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_h,      moveresize,     {.v = "0x 0y -25w 0h" } },
	{ MODKEY|ControlMask,           XK_k,      moveresizeedge, {.v = "t"} },
	{ MODKEY|ControlMask,           XK_j,      moveresizeedge, {.v = "b"} },
	{ MODKEY|ControlMask,           XK_h,      moveresizeedge, {.v = "l"} },
	{ MODKEY|ControlMask,           XK_l,      moveresizeedge, {.v = "r"} },
	{ MODKEY|ControlMask|ShiftMask, XK_k,      moveresizeedge, {.v = "T"} },
	{ MODKEY|ControlMask|ShiftMask, XK_j,      moveresizeedge, {.v = "B"} },
	{ MODKEY|ControlMask|ShiftMask, XK_h,      moveresizeedge, {.v = "L"} },
	{ MODKEY|ControlMask|ShiftMask, XK_l,      moveresizeedge, {.v = "R"} },

	STACKKEYS(MODKEY,                          focus)
	STACKKEYS(MODKEY|ShiftMask,                push)

	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },

	{0, XF86XK_Display,                        spawn,          SHCMD("scrmenu") },
	{0, XF86XK_AudioMute,                      spawn,          SHCMD("lmc toggle && ( pkill -RTMIN+10 \"${STATUSBAR:-dwmblocks}\" >/dev/null 2>&1 ) 2>/dev/null") },
	{0, XF86XK_AudioLowerVolume,               spawn,          SHCMD("lmc down && ( pkill -RTMIN+10 \"${STATUSBAR:-dwmblocks}\" >/dev/null 2>&1 ) 2>/dev/null") },
	{0, XF86XK_AudioRaiseVolume,               spawn,          SHCMD("lmc up && ( pkill -RTMIN+10 \"${STATUSBAR:-dwmblocks}\" >/dev/null 2>&1 ) 2>/dev/null") },
	{0, XF86XK_AudioMicMute,                   spawn,          SHCMD("lmc mictoggle && ( pkill -RTMIN+10 \"${STATUSBAR:-dwmblocks}\" >/dev/null 2>&1 ) 2>/dev/null") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5} },
	{ ClkStatusText,        ShiftMask,      Button1,        sigdwmblocks,   {.i = 6} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

