THINGS TO CHANGE
    Should be reactions to missing targets or hitting all targets

Cast
    Admiral Rakba - he sends you on your mission
    Randos - when active there are random NPCs quickly moving all around the ship taht you can't talk to.
    Buddy - your buddy on the ship
    Ruiz - Another TACCOM gunner, highly indoctrinated
    Chielf Drive Engineer Lee - chief engineer, always on duty
    Tactical Commander Smith - your boss, always seems angry
    Captain Lassic - usually jovial
    Intelligence Officer Tari - shifty

SCRENE MODES
    WALKING
        DESCRIPTION
            tiled map
            can walk along grid?
            walk into an NPC to talk to it?
        UI ELEMENTS
            SCENE VIEW 
                MAP - grid based tiled map of the entire level
                LEVEL SPECIFIC MAP NPCS - anything interactive is an NPC that can be walked into to trigger
                NPCs - traditional NPCs that can stand or move along a path, can be walked into to talk to.
            SHIP OVERVIEW - graphic of all the decks of the ship with your current deck highlighted
            D-PAD - bottom left d-pad that works with touch or mouse for moving player
    BATTLE
        DESCRIPTION
            camera view of planet
            looking straight down on it
            have layers for the ground, multiple options layers of clouds, HUD targeters and markers on top of everything
        UI ELEMENTS
            CAMERA VIEW - layered view of planet surface with the surface and option cloud layers
            HUD OVERLAY - 
                ENEMY MARKERS
                FRIENDLY MARKERS
                INFO BLOCK
                TARGETING RETICLE
                APPROX IMPACT MARKER
                APPROX WARHEAD YIELD IMPACT RADIUS
            D-PAD - bottom left d-pad that works with touch or mouse to move targeting reticle
            FIRE BUTTON - bottom right button that fires a warhead
            YIELD SELECTOR - buttons down right side of the screen to select warhead yield (touch or mouse, number keys should work too)
            SHIP STATUS DISPLAY - readout on top of screen - displays ship status (alerts that are usually part of cutscenes)
            TARGET STATUS DISPLAY - list of targets on screen with count of how many good or bad guys are destroyed
            MISSION TIME - bottom middle clock counting down time until ship is out of range of target
    CUTSCENE
        DESCRIPTION
            one image
            scrolling or writing text
        UI ELEMENTS
            IMAGE - image related to what the cutscene is about
            TEXT - the text to be drawn
    ENGINE ROOM
        DESCRIPTION
            Modified WALKING scene
            If there is times build an engine room map
            Small walk way over massive spinning gears
            They spin at varying rates in a prolonged startup sequence
            Can be spun up until a massive explosion is triggered, overload that destroys the ship
        UI ELEMENTS
            SCENE VIEW
                GEARS - multiple layers of gears in varying sizes that turn at different rates, below the map layer
                MAP - the map the player can walk on is small and "above" the gears
                LEVEL - level specific NPCs just like WALKING scene
                NPCS - traditional NPCs just like the WALKING SCENE
Structure
    Sol System - The Beginning
        GOTO - SCENE_01_WALKING_INTRO
        [SCENE_01_WALKING_INTRO] - New on the ship
            Cutscene
                Intro - text crawl introduces the ship and what it's for.
            NPCs
                Buddy - will talk with you about being excited to be doing danger close missions.
                Lee - welcomes you to the ship
                Smith - tells you to get rest because tomorrow is practice
            Events
                SLEEP - you can trigger sleep by going to bed
                PRACTICE - 
                    TRIGGER - wake up after sleep
                    GOTO - SCENE_02_BATTLE_TACCOM_PRACTICE_1
        [SCENE_02_BATTLE_TACCOM_PRACTICE_1] - taccom practice 1
            Battle
                Zoomed out map shows europe
                Zoomed in map shows:
                    targets: manufacturing facilities, enemy bases
                    non-targets: schools, housing, friendly troops
                Bomb manufacturing facilities and enemy bases.                
            Cutscene
                After Battle - Alert about STRATCOM simulation being underway, witness simulated STRATCOM strike on Europe
            Events
                LEAVE 
                    TRIGGER - after cutscene
                    GOTO - SCENE_03_WALKING_AFTER_PRACTICE_1
        [SCENE_03_WALKING_AFTER_PRACTICE_1] - After first practice
            NPCs
                Buddy - will tell you "That was close! you dropped it right in the pickle jar!"
                Smith - will tell you you did ok, that you got it very close to friendlies, rest up for more practice tomorrow.
                Tari & Smith arguing 
                    Tari is mad that he wasn't told Smith's trainees were logged in during the STRATCOM test, tells him to never let it happen again. 
                    Smith protests the tone but Captain Lassic informs him that Tari has the highest priority on the ship.
                Stratcom Door - locked with red light
            Events
                SLEEP - you can trigger by going to bed
                PRACTICE - 
                    TRIGGER - triggered when you come back from sleep
                    GOTO - SCENE_04_BATTLE_TACCOM_PRACTICE_2
        [SCENE_04_BATTLE_TACCOM_PRACTICE_2] - taccom practice 2 - closer quarters
            Battle
                Zoomed out map shows generic island
                Zoomed in map shows:
                    enemy positions and vehicles
                    friendly positions and vehicles
                Bomb enemy positions and vehicles
            EVENTS
                GOTO - SCENE_05_WALKING_AFTER_PRACTICE_2
        [SCENE_05_WALKING_AFTER_PRACTICE_2] - 
            NPCs
                Randos - activated
                Buddy - says there's an actual battle in Europe, must be serious because stratcom is activated
                Lee - Can't talk, stratcom wants full power for some reason
                Smith - Get to your stations and don't go near stratcom, Tari is on a rampage!
                Stratcom Door - armed guards around it
            EVENT - 
                BATTLE - 
                    TRIGGER - triggered by talking to smith
                    GOTO - SCENE_06_BATTLE_TACCOM_MISSION_1
        [SCENE_06_BATTLE_TACCOM_MISSION_1] - taccom battle mission 1
            Battle
                Zoomed out map shows europe
                Zoomed in map shows:
                    Tiny village with a handful of enemies and a handful of allies
                Drop 1 small bomb 
            Cutscene
                red alert triggered
                return to zoomed out map
                See strike on Europe that matches STRATCOM simulated strike
                Fade to black
            EVENT
                TRIGGER - after cutscene
                GOTO - SCENE_07_WALKING_FTER_BATTLE_1
        [SCENE_07_WALKING_AFTER_BATTLE_1] - After the attack
            Cutscene
                Alien attack - text crawl of message from captain explaining that an unprovoked attack from previously unknown alien species wiped out millions
                SNS Ernest King Refit - the Ernest King will be fitted with stasis pods and a new drive and be sent to destroy the threat
            NPCs
                Randos - activated
                Buddy - 
                    scared about how long they'll be away from earth, 
                    where the heck is Echo 7? 
                    So weird that we were called to a tiny battle and just happened to be right by where the alien attack was
                Tari - Says "That was close, we were nearly hit by the projectile that destroyed europe, it practically passed through our exact position"
                Smith - you are off duty, you seem like a good person. I will never ask you do to something I know is immoral. Trust your gut. Do what is right.
            EVENT
                SLEEP - 
                    TRIGGER - can be triggered by going to bed   
                    GOTO - SCENE_08_WALKING_AFTER_JUMP
    Echo 7 - Something is very wrong
        [SCENE_08_WALKING_AFTER_JUMP] - After the jump
            Cutscene
                After the jump - text crawl explaining that you've been in stasis as the Ernest King travels to Echo 7 for the first attack of the new interstellar war.
            NPCs
                Buddy - so groggy
                Smith - 
                    there's tremendous cloud cover
                    Tari scanned the planet and marked targets, 
                    We'll go on instruments
                    We'll let you know when it's time for the attack run.
                Lee - doesn't understand how the engines took so much wear in such a short amount of time
                Tari - won't even make eye contact
            EVENT
                SLEEP - 
                    TRIGGER - can be triggered by going to bed
                BATTLE - 
                    TRIGGER - triggers after sleep
                    GOTO - SCENE_09_BATTLE_TACCOM_MISSION_2
        [SCENE_09_BATTLE_TACCOM_MISSION_2] - Taccon Battle Mission 2 - Echo 7
            BATTLE
                Zoomed out view shows a surface of multicolored clouds
                Zoomed in view shows 
                    numerous layers of cloud moving past at different speeds, all fast
                    HUD markers for targets to bomb
                Bomb the markers
            EVENT
                TRIGGER - mission is over
                GOTO - SCENE_10_WALKING_AFTER_BATTLE_2
        [SCENE_10_WALKING_AFTER_BATTLE_2] - after attack on ECHO 7
            NPCS
                Ruiz - That was close! We almost lost the battle! (what the fuck is this guy talking about?)
                Tari - Congratulates you on your amazing skills, regardless of how many targets you hit.
                Smith - 
                    If you hit everything he says you did a good job, 
                    if you missed any he says didn't get all the shots, but wonders why nobody seems to care
            EVENT
                CALL TO BRIDGE - you are called to bridge (use cutsceen screen to show the alert?)
                    TRIGGER -  triggers after you've talked to everybody you can
                GO TO BRIDGE - 
                    TRIGGER - triggers when you go to the elevator
                    GOTO - SCENE_11_WALKING_BRIDGE_1
        [SCENE_11_WALKING_BRIDGE_1] - On the Bridge after Echo 7
            NPCs
                Bridge Crew - there are people at the different stations doing things
                Lassic - Commends your performance and lets you know that if you keep following orders and stick to your duties you may be promoted to STRATCOM
                Bridge Command Console - shows engine status, weapons status, etc
                Bridge Navigation Console - 
                    Lit up to show a navigation chart
                    Waypoints on it are labeled:
                        SOL (home)
                        ECHO 7
                        OSCAR 2
                        NOVEMBER 3
                        [ANOMALY]
            EVENT
                GET CALLED OUT - captain calls you out and tells you to leave the bridge
                    TRIGGER - after you look at the navigation console
                GO TO CREW DECK - go back to the normal ship area
                    TRIGGER - touch the elevator
                    GOTO - SCENE_12_WALKING_AFTER_BRIDGE_1
        [SCENE_12_WALKING_AFTER_BRIDGE_1] - walking around the crew deck after being on the bridge
            NPCS
                Buddy - talk about what it means that they are going to an anomaly, he says he heard stuff that troubles him a lot about what they are doing.
                Ruiz - reacts negatively to your tone about the anomaly, he is sure it's for the good of mankind.
                Smith - tells you to watch your back, he has a bad feeling about all of this
                Lee - 
                    the engines are beat to shit 
                    if they pushed them above 90% now there'd be a reactor overload
            EVENT
                SHOW CUTSCENE - show the cutscene for the next jump
                    TRIGGER - everyone has been talked to?
                    GOTO - SCENE_13_WALKING_AFTER_JUMP_2
    OSCAR 2 - The true face of your enemy
        [SCENE_13_WALKING_AFTER_JUMP_2]
            CUTSCENE - 
                After the jump 2 - the Ernest King is now at OSCAR 2 and due to security concerns the crew must be much more careful about sharing information of what they are working on.
            NPCS
                lee
                    Something is very wrong. The engines have deteriorated far greater than even the first jump. Pushing beyond 70% would cause a reactor overload now
                smith - 
                    tells you buddy died in stasis during the jump
                    the next mission will have complete cloud cover again
                    working on instruments only
                    get some rest
                ruiz - can't talk to you, the captain said we have to tighten our lips
            EVENT
                SLEEP - go to sleep
                    TRIGGER - go to the bed
                    GOTO - SCENE_14_BATTLE_TACCOM_MISSION_3
        [SCENE_14_BATTLE_TACCOM_MISSION_3]
            BATTLE
                ZOOMED OUT view is complete cloud cover but different color clouds
                ZOOMED IN VIEW is lots of cloud cover with HUD targeters
                30 seconds into the mission the clouds begin clear and you see what looks like a very basic farming village
                Your character should have internal dialog saying "wtf is this"
            EVENT
                END BATTLE 
                    TRIGGER - all targets destroyed or time elapses
                    GOTO - SCENE_15_WALKING_AFTER_BATTLE_3
        [SCENE_15_WALKING_AFTER_BATTLE_3]
            Cutscene
                You didn't see that - Tari tells you that whatever you think you saw, you didn't. Keep your mouth shut. You're confined to quarters until your next mission
            SET YOUR POSITION - you are locked in your room.
            NPCs
                Bed - removed and replaced with map block bed.
                Door - removed and replaced with map block door.
            EVENT
                RUIZ ARRIVES - Brings you food. That was close! The offical message is that we narrowly achieved victory, and that's the only message I'm interested in listening to!
                    TRIGGER - 30 SECONDS after scene starts
                SMITH ARRIVES - Here to debrief you, you tell him what you saw, he says he'll get to the bottom of this.
                    TRIGGER - 30 seconds after RUIZ leaves
                BED IS ACTIVE - the bed map block is replaced with bed NPC
                    TRIGGER - after Smith leaves
                SLEEP - go to sleep
                    TRIGGER - touch the bed
                    GOTO - SCENE_16_WALKING_AFTER_CONFINEMENT
        [SCENE_16_WALKING_AFTER_CONFINEMENT]
            Cutscene
                PROMOTION - Captain says they've reviewed your performance and decided to promote you to STRATCOM. And they regret that smith has committed suicide. You now report directly to TARI.
                STRATCOM BRIEFING - We don't have any more big targets so for now I want you to rest and avoid talking to anybody. After the next jump you'll be needed.
            NPCs
                Ruiz - won't talk to you
                Lee - knows something's wrong but needs more data
                Tari - tells you to rest for the next jump
            EVENTS
                SLEEP - go to sleep
                    TRIGGER - touch the bed
                    GOTO - SCENE_17_WALKING_AFTER_JUMP_3
    NOVEMBER 3 - The mutiny
        [SCENE_17_WALKING_AFTER_JUMP_3]
            Cutscene
                After the jump 3 - Captain says We've jumped to NOVEMBER 3, the final planet.
            NPCs
                Lee - 
                    He ran an isolated system off the grid while we were in stasis. 
                    The reactor output far higher levels than they were programmed to.
                    Pushing above 50% will cause a reactor overload.
                    We can't make it home now.
                    He believes Tari is responsible and they need to go to the bridge to tell the captain
            EVENTS
                GO TO BRIDGE
                    TRIGGER - happens after talking to Lee
                    GOTO - SCENE_18_WALKING_BRIDGE_MUTINY
        [SCENE_18_WALKING_BRIDGE_MUTINY]
            Cutscene
                Mutiny - 
                    lee - tells the captain what he found and he believes Tari is manipulating the mission
                    lassic - explains that he's the captain and he knows exactly what is happening on his ship
                    tari - explains the attack on europe was conducted by the Ernest King in secret to trick the world into investing the technology to be able to travel this far
                    lassic - explains they are approach an anomaly emitting a kind of data that, if studied closely, could give unlimited power to whoever has it
                    tari - they are going to take that and bring it back to the military leaders that arranged the mission and take control of earth for good.
                    lassic - there will be no more war because all people will be governed by a new military based world government
            Cutscene - in game somehow?
                overload the reactor
                    bridge crew captures lee
                    lee tells you to overload the reactor or the world will be taken over by them
        [SCENE_19_WALKING_OVERLOAD_THE_REACTOR]
                you have to get to the door then get to engineering and trigger an overload 
                INSERT AWESOME REACTOR OVERLOAD SCENE IF IT EXISTS
    SOL
        [SCENE_20_GOD_AMONGST_MEN]
            Cutscene
                That was Close - 
                    Admiral Rakba - 
                        THAT WAS CLOSE he says
                        The readings they are getting suggests that the Ernest King suffered a reactor overload mere moments after the data was transmitted back to him.
                        But he has the data now.
                        And the world will be mine to rule.