/* including x11 keysym for volume/brightness keys */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 16;        /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const char *fonts[]          = { "Inconsolata:size=12", "NotoColorEmoji:size=12" };
static const char dmenufont[]       = "Inconsolata:size=12";
static const char base00[]          = "#181818";
static const char base01[]          = "#282828";
static const char base02[]          = "#383838";
static const char base03[]          = "#585858";
static const char base04[]          = "#b8b8b8";
static const char base05[]          = "#d8d8d8";
static const char base06[]          = "#e8e8e8";
static const char base07[]          = "#f8f8f8";
static const char base08[]          = "#ab4642";
static const char base09[]          = "#dc9656";
static const char base0A[]          = "#f7ca88";
static const char base0B[]          = "#a1b56c";
static const char base0C[]          = "#86c1b9";
static const char base0D[]          = "#7cafc2";
static const char base0E[]          = "#ba8baf";
static const char base0F[]          = "#a16946";
static const unsigned int baralpha = 0xda;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*               fg      bg      border   */
	[SchemeNorm] = { base04, base00, base01 },
	[SchemeSel]  = { base05, base02, base0C },
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { "Z", "X", "C", "A", "S", "D", "Q", "W", "E", "~" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                instance    title       tags mask     switchtotag    isfloating   monitor */
	{ "LibreWolf",          NULL,       NULL,       1 << 0,       1,             0,           -1 },
	{ "Falkon",             NULL,       NULL,       1 << 0,       1,             0,           -1 },
	{ "krita",              NULL,       NULL,       1 << 6,       1,             0,           -1 },
	{ "mpv",                NULL,       NULL,       1 << 1,       1,             0,           -1 },
	{ "krusader",           NULL,       NULL,       1 << 4,       1,             0,           -1 },
	{ "okular",             NULL,       NULL,       1 << 8,       1,             0,           -1 },
	{ "calibre",            NULL,       NULL,       1 << 8,       1,             0,           -1 },
	{ "SpeedCrunch",        NULL,       NULL,       0,            0,             1,           -1 },
	{ "discord",            NULL,       NULL,       1 << 2,       1,             0,           -1 },
	{ "Steam",              NULL,       NULL,       0,            0,             1,           -1 },
	{ "XCalendar",          NULL,       NULL,       0,            0,             1,           -1 },
	{  NULL,                NULL,       "scratch",  1 << 9,       0,             1,           -1 },
	{  NULL,                NULL,       "ncmpcpp",  1 << 5,       1,             0,           -1 },
	{  NULL,                NULL,       "newsboat", 1 << 5,       1,             0,           -1 },
};

/* layout(s) */
#include "layouts.c"
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[T]",      tile },    /* first entry is default */
	{ "[F]",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "[G]",      grid },
	{ "[B]",      bstack },
	{ "[H]",      bstackhoriz },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0";
static const char *dmenucmd[] = { "dmenu_run", "-b", "-m", dmenumon, "-fn", dmenufont, "-nb", base00, "-nf", base04, "-sb", base02, "-sf", base05, NULL };
static const char *krunner[] = { "krunner", NULL };
static const char *xkill[] = { "xkill", NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *termtabbed[]  = { "st-tabbed.sh", NULL };
static const char *browser[]  = { "falkon", NULL };
static const char *browser2[]  = { "surf-open.sh", NULL };
static const char *browser3[]  = { "librewolf-bin", NULL };
static const char *pulse_switch[]  = { "switch-audio.sh", NULL };
static const char *mpc_pause[]  = { "mpc", "toggle", NULL };
static const char *mpc_next[]  = { "mpc", "next", NULL };
static const char *mpc_prev[]  = { "mpc", "prev", NULL };
static const char *mpc_stop[]  = { "mpc", "stop", NULL };
static const char *mpc_replay[]  = { "mpc", "seek", "0%", NULL };
static const char *mpc_back[]  = { "mpc", "seek", "+5", NULL };
static const char *mpc_forward[]  = { "mpc", "seek", "-5", NULL };
static const char *ncmpcpp[]  = { "st", "-t", "ncmpcpp", "-e", "ncmpcpp", NULL };
static const char *newsboat[]  = { "st", "-t", "newsboat", "-e", "newsboat", NULL };
static const char *discord[]  = { "discord", NULL };
static const char *steam[]  = { "nv", "steam", NULL };
static const char *imgeditor[]  = { "krita", NULL };
static const char *vscode[]  = { "vscode", NULL };
static const char *torrentclient[]  = { "qbittorrent", NULL };
static const char *calendar[] = { "xcalendar", NULL };
static const char *sys_mon[]  = { "st", "-e", "htop", NULL };
static const char *display_setup2[]  = { "dual-monitor-setup.sh", NULL };
static const char *calc[]  = { "speedcrunch", NULL };
static const char *mixer[]  = { "pavucontrol", NULL };
static const char *mixer2[]  = { "st", "-e", "alsamixer", NULL };
static const char *filebrowser[]  = { "krusader", NULL };
static const char *screenshot[]  = { "scrot",  NULL };
static const char *lockscreen[]  = { "sudo", "slock", NULL };
static const char *suspend[] = { "sudo", "s2ram", NULL};
static const char *suspend_lock[] = { "/home/dylan/apps/bin/lock+suspend.sh", NULL};
static const char *reboot[] = { "sudo", "reboot", NULL};
static const char *poweroff[] = { "sudo", "poweroff", NULL };
static const char *upvol[]   = { "amixer", "-D", "pulse", "sset", "Master", "5%+", NULL };
static const char *downvol[] = { "amixer", "-D", "pulse", "sset", "Master", "5%-", NULL };
static const char *mutevol[] = { "amixer", "-D", "pulse", "sset", "Master", "mute", NULL };
static const char *upbacklight[] = { "sudo", "xbacklight", "-inc", "10", NULL };
static const char *downbacklight[] = { "sudo", "xbacklight", "-dec", "10", NULL };
static const char *scratch[] = { "st", "-g", "239x25+0+0", "-t", "scratch", NULL };
static const char *playsong[] = { "PlaySong.sh", NULL };
#include "push.c"

static const Key keys[] = {
	/* modifier                     key        function        argument */

  /* Launching applications + media + system keys */
	{ MODKEY,                   XK_BackSpace,  spawn,          {.v = xkill } },
	{ MODKEY,                       XK_space,  spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask|ControlMask, XK_space,  spawn,          {.v = krunner } },
	{ MODKEY,                       XK_1,      spawn,          {.v = browser } },
	{ MODKEY|ShiftMask,             XK_1,      spawn,          {.v = browser2 } },
	{ MODKEY|ControlMask,           XK_1,      spawn,          {.v = browser3 } },
	{ MODKEY,                       XK_2,      spawn,          {.v = filebrowser } },
 	{ MODKEY,                       XK_3,      spawn,          {.v = calc } },
	{ MODKEY,                       XK_4,      spawn,          {.v = mixer } },
	{ MODKEY|ControlMask,           XK_4,      spawn,          {.v = mixer2 } },
	{ MODKEY|ShiftMask,             XK_4,      spawn,          {.v = pulse_switch } },
	{ MODKEY,                       XK_5,      spawn,          {.v = imgeditor } },
	{ MODKEY|ShiftMask,             XK_5,      spawn,          {.v = vscode } },
 	{ MODKEY,                       XK_6,      spawn,          {.v = discord } },
 	{ MODKEY|ShiftMask,             XK_6,      spawn,          {.v = steam } },
 	{ MODKEY,                       XK_7,      spawn,          {.v = torrentclient } },
 	{ MODKEY,                       XK_8,      spawn,          {.v = playsong } },
 	{ MODKEY,                       XK_9,      spawn,          {.v = calendar } },
 	{ MODKEY,                       XK_p,      spawn,          {.v = display_setup2 } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termtabbed } },
	{ MODKEY|ControlMask,           XK_Return, spawn,          {.v = termcmd } },
 	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = scratch } },
	{ MODKEY,                       XK_Escape, spawn,          {.v = sys_mon } },
 	{ MODKEY,                       XK_F9,     spawn,          {.v = lockscreen } },
 	{ MODKEY,                       XK_F10,    spawn,          {.v = suspend } },
 	{ MODKEY|ShiftMask,             XK_F10,    spawn,          {.v = suspend_lock } },
	{ MODKEY,                       XK_F11,    spawn,          {.v = reboot} },
	{ MODKEY,                       XK_F12,    spawn,          {.v = poweroff} },
 	{ 0,              XF86XK_AudioLowerVolume, spawn,          {.v = downvol} },
 	{ 0,              XF86XK_AudioRaiseVolume, spawn,          {.v = upvol} },
 	{ 0,              XF86XK_AudioMute,        spawn,          {.v = mutevol} },
 	{ 0,              XF86XK_MonBrightnessUp,  spawn,          {.v = upbacklight} },
 	{ 0,            XF86XK_MonBrightnessDown,  spawn,          {.v = downbacklight} },
 	{ 0,              XK_Print,                spawn,          {.v = screenshot} },
 	{ MODKEY,                      XK_F1,      spawn,          {.v = mpc_pause} },
 	{ MODKEY,                      XK_F2,      spawn,          {.v = mpc_prev} },
 	{ MODKEY,                      XK_F3,      spawn,          {.v = mpc_next} },
 	{ MODKEY,                      XK_F4,      spawn,          {.v = mpc_stop} },
 	{ MODKEY,                      XK_F5,      spawn,          {.v = mpc_replay} },
 	{ MODKEY,                      XK_F6,      spawn,          {.v = mpc_forward} },
 	{ MODKEY,                      XK_F7,      spawn,          {.v = mpc_back} },
 	{ MODKEY,                      XK_F8,      spawn,          {.v = ncmpcpp} },
 	{ MODKEY|ShiftMask,            XK_F8,      spawn,          {.v = newsboat} },

  /* dwm operations (layout modes, killing windows, resize master, etc) */
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_y,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ControlMask,           XK_space,  setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_n,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY|ShiftMask,             XK_n,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY,                       XK_r,      setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_u,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = -0.10} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = +0.10} },
	{ MODKEY|ShiftMask,          XK_semicolon, setcfact,       {.f =  0.00} },
	{ MODKEY,                    XK_semicolon, resetmfact,     {0} },
	{ MODKEY,                       XK_v,      zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_f,      killclient,     {0} },
  { MODKEY|ShiftMask,             XK_k,      pushup,         {.i = -1 } },
  { MODKEY|ShiftMask,             XK_j,      pushdown,       {.i = +1 } },
  { MODKEY,                       XK_o,      winview,        {0} },
	{ MODKEY|ShiftMask,             XK_f,      quit,           {0} },

  /* Tag/monitor manipulation */
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_z,                      0)
	TAGKEYS(                        XK_x,                      1)
	TAGKEYS(                        XK_c,                      2)
	TAGKEYS(                        XK_a,                      3)
	TAGKEYS(                        XK_s,                      4)
	TAGKEYS(                        XK_d,                      5)
	TAGKEYS(                        XK_q,                      6)
	TAGKEYS(                        XK_w,                      7)
	TAGKEYS(                        XK_e,                      8)
	TAGKEYS(                        XK_grave,                  9)
};

/* (mouse) button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button2,        setlayout,      {.v = &layouts[1]} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkStatusText,        0,              Button1,        spawn,          {.v = termcmd } },
	{ ClkStatusText,        0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button3,        killclient,     {0} },
	{ ClkStatusText,        0,              Button4,        focusstack,     {.i = -1} },
	{ ClkStatusText,        0,              Button5,        focusstack,     {.i = +1} },
	{ ClkStatusText,        MODKEY,         Button1,        togglebar,      {0} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            0,              Button2,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
