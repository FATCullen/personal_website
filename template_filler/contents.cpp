#include "contents.h"

const extern std::string ABOUT_PROFESSIONAL = 
    "I study software engineering at the University of Waterloo, and I'm currently interning at Nokia for firmware development.";
const extern std::string ABOUT_PERSONAL = 
    "In my free time I help teach my university's Judo club, and recently I've mostly been playing Dwarf Fortress and reading parts of Terry Pratchett's 'Discworld.'";

const extern std::string FAVOURITE_LANGUAGES  = "C++, Python, JavaScript, Java";
const extern std::string FAVOURITE_FRAMEWORKS = "React, Spring, SQL, Docker, Flask, AWS, ROS";
const extern std::string SKILLS               = "CAD, Soldering, Circuit design";

const extern std::string GPA = "GPA - 3.8";

const extern std::vector<Project> PROJECTS = {
    {
        "/images/projects/walter.jpg",
        "WalTer Bot",
        "Navigates and captures images from a room full of electronic components, then categorizes them via image processing.  Provides an LLM interface to help you plan hardware projects with the found parts.",
        {
            {"Demo", "https://www.youtube.com/watch?v=k7Ha7dAPiZ8"}, 
            // {"Source", ""}, 
            {"DevPost", "https://devpost.com/software/walter-your-everyday-inventory-companion"}, 
        }
    },
    {
        "/images/projects/debacle.jpg",
        "Debug Debacle",
        "Multiplayer competitive coding game, where users compete to correct bugs in code fastest. Built for MCHacks 2025, winning 3rd place overall.",
        {
            {"Demo", "https://debug-debacle.vercel.app/"}, 
            {"Source", "https://github.com/honzikschenk/debug-debacle"}, 
            {"DevPost", "https://devpost.com/software/debug-debacle"}, 
        }
    },
    {
        "/images/projects/pathmaxer.jpg",
        "PathMaxer",
        "Campus tour guide robot. Users upload class schedule from a smartphone app, and the robot guides them between classes. Uses Dijkstra's algorithm for pathfinding.",
        {
            {"Demo", "https://drive.google.com/file/d/1TNJl6DPzvSaDpCO_igs-X81bE8GjgqE4/view"}, 
            // {"Source", ""}, 
            // {"DevPost", ""}, 
        }
    },
    {
        "/images/projects/utra.jpg",
        "UTRA Hacks Robot",
        "Built for UTRA Hacks 2025 \"capture the flag\" style challenges. Lots of struggles with the limited sensor array, and very little sleep, but we pulled through and placed 2nd overall.",
        {
            // {"Demo", ""}, 
            {"Source", "https://github.com/vichua2006/UTRAHack2025/tree/main"}, 
            {"DevPost", "https://devpost.com/software/lebot-james-qgf9kw"}, 
        }
    },
    {
        "/images/projects/thyone.jpg",
        "Thyone Consulting Website",
        "Freelance web design work for the Thyone Consulting group. React site with a headless CMS for easy content updates.",
        {
            {"Demo", "https://thyoneconsulting.com/"}, 
            // {"Source", ""}, 
            // {"DevPost", ""}, 
        }
    },
    {
        "/images/projects/threed.jpg",
        "3D Graphics Engine",
        "For whatever reason I like making 3D renderers, so I've built several.  My latest is written in C++, with OBJ file support, hidden face occlusion, and basic lighting.",
        {
            // {"Demo", ""}, 
            {"Source", "https://github.com/FATCullen/3d_engine_stuff"}, 
            // {"DevPost", ""}, 
        }
    },
};

const extern std::vector<Work> WORKS = {
    {
        "/images/logos/nokia.svg",
        "DSP Firmware Engineer - Nokia",
        "Fall 2026",
        "Working on DSP SDK for pluggable optical engines",
    },
    {
        "/images/logos/brainridge.svg",
        "Software Engineering Consultant - BrainRidge",
        "Winter 2026",
        "FinTech apps for companies including BMO and Interac",
    },
    {
        "/images/logos/fencesee.svg",
        "Full Stack Developer - FenceSee",
        "Summer 2025",
        "SaaS for planning fencing jobs",
    },
    {
        "/images/logos/mina.svg",
        "Research Assistant - MiNa Labs",
        "Summer 2023",
        "University of Victoria microfluidics and nanotech lab",
    },
};

const extern std::vector<WebBadge> WEB_BADGES = {
    {
        "/images/webbadges/webring.png",
        "https://se-webring.xyz",
    },
    // {
    //     "/images/webbadges/rss.png",
    //     "/",
    // },
    {
        "/images/webbadges/github.png",
        "https://github.com/FATCullen",
    },
    {
        "/images/webbadges/linkedin.png",
        "https://www.linkedin.com/in/finn-cullen-a6a012346/",
    },
    {
        "/images/webbadges/email.png",
        "mailto:fatcullen2@gmail.com",
    },
    // {
    //     "/images/webbadges/gemini.png",
    //     "/",
    // },
    // {
    //     "/images/webbadges/gopher.png",
    //     "/",
    // },
};

const extern std::vector<Blog> BLOGS = {
    {
        "2026-09-21",
        "Xiji Island - Test",
        "xiji_island_test",
        {
            "Geography",
        }
    },
};