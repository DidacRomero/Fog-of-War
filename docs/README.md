## Fog of War Research (Project 2)

“I am [Dídac Romero](https://www.linkedin.com/in/d%C3%ADdac-romero-cam%C3%B3-203692166/ "LinkedIn"), student of the [Bachelor’s Degree in Video Games by UPC at CITM](https://www.citm.upc.edu/ing/estudis/graus-videojocs/ "Link to the Degree"). This content is generated for the second year’s subject Project 2, under supervision of lecturer [Ricard Pillosu](https://es.linkedin.com/in/ricardpillosu "LinkedIn").”

## What is Fog Of War?
In origin, Fog of War is a term coined and initially used by military officers. Fog of War refers to the uncertainty from difficulty of accurate recognition of your adversaries capabilities, terrain layout and your capabilities to react to the problems that may arise due to this uncertainty.

So in essence, Fog of War is the lack of accurate information regarding a strategic factor you can’t control. 

Since uncertainty can be a very powerful mechanic of a game, specially in strategy games we will now see how the concept of Fog of War is applied to video games both in terms of design and implementation.

## Fog of War in video games

#### First appearance

The first time we saw the concept of Fog of War applied in a video game was in the Turn-based Wargame [_Empire_](https://en.wikipedia.org/wiki/Empire_(1977_video_game)) by [_Walter Bright_](https://en.wikipedia.org/wiki/Walter_Bright). In this game black squared tiles covered unvisited areas.

<img src="Images/EmpireOriginal.png" style="display: block; margin: auto;" />

#### Classic Fog of War in Strategy Games
[_Warcraft II: Tides of Darkness_](https://en.wikipedia.org/wiki/Warcraft_II:_Tides_of_Darkness) by [_Blizzard Entertainment_](https://www.blizzard.com/en-us/) presents an expansion in the concept of Fog of war in video games by adding a “fogged zone”, in which, if you have scouted that area, you’ll be able to see the map but not the enemies that might be moving or preparing an attack under its cover. 

![WarcraftII FOW](Images/Warcraft%20II%20FOW.gif "Warcraft 2 Fog of War system")

From that moment on, later strategy games such as _Age of Empires_ or _League of Legends_ to list a few, will implement fogged areas, and these games will let the fogged state reveal information in different ways. For instance, in Age of Empires, you may scout an area where there’s a building, and once you leave the area you still see the building with its life points at the moment of sight. But when you come back the building might have been demolished entirely, and you will only have that information if you re-scout the area with your units. 

#### League of Legends
In [_League of Legends_](https://en.wikipedia.org/wiki/League_of_Legends) by [_Riot Games_](https://www.riotgames.com/en) all areas of the map where you don’t have a structure or ally providing visibility are covered in a fog that allow you to see the terrain but not the enemy or neutral entities that are inside the fogged area. But even though you may not have vision of neutral entities like a Drake or the Baron Nashor, which provide great buffs when killed, the game will alert players if these monsters are killed by your opponents. League of Legends is applying the concept of giving information to the players that they shouldn't have in the first place. It seems counterintuitive to implement such a feature for a game in which Fog of War plays a critical role, but that feature may help to avoid delivering a bad gaming experience, since the improvements of these buffs change for a period of time the strategies that both teams will follow and facing the enemy team without that information will more often than not lead to a crushing defeat.

#### Innovative ways to display Fog of War 
Now we will check some innovative ways that can still be related to the original concepts of completely unvisited and fogged areas.
##### Recompile
In the still to be released video game [_Recompile_](https://recompilegame.com/) by [_Phigames_](https://phigames.co.uk/), a new way of revealing the map is explored, in which you “undistort” the world to discover which paths to follow.

Their explanaition on how they achieved this effect [here](https://phigames.co.uk/rcfog.html).

![Recompile](Images/Recompile%20FOW.gif "Isn't Recompile Fog of War really cool?")



##### Cylindrus
[_Cylindrus_](https://shakethatbutton.com/cylindrus/) by [_Jerry Belich_](https://jerrytron.com/) is a game played on a cylinder, creating the effect of “natural fog of war” as described in this [Gamasutra article](http://www.gamasutra.com/view/news/291029/AltCTRLGDC_Showcase_Jerry_Belichs_Cylindrus.php), which forces you to move around the cylinder to see what’s going on.

<iframe width="560" height="315" src="https://www.youtube.com/embed/indWEhAzGac?start=40" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

##### Ape Out
To mirror [Ape Out](https://www.devolverdigital.com/games/view/ape-out) by [_Bennett Foddy_](https://en.wikipedia.org/wiki/Bennett_Foddy), [_Gabe Cuzzillo_](https://twitter.com/gabecuzzillo?lang=en) and [Matt Boch](https://gamecenter.nyu.edu/faculty/matt-boch/) perspective, we can create the same environment characteristics by combining a 2D map with a technique we will talk about later called 2D visibility, the fog of would help us give a cool perspective to the game.

<iframe width="560" height="315" src="https://www.youtube.com/embed/K7jRL2MtHmU?start=33" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

--- 
## Technical approaches
We have seen now which aspects are comprised in the use of Fog of War mechanics and design. Now we will look into how we can implement this features and how the different ways to implement fog of war will help us execute the design we have in mind for our Fog of War mechanics.

### Tile based approach
Some games maps, specially 2D games, are made up of tiles which can have different shapes: orthogonal squared tiles, isometric rectangle tiles or hexagonal tiles. Note that you can still apply this approach to a game with different characteristics in terms of the map, but it will need a way to abstractly partition the terrain that more often than not will make it harder to implement this method.

![Tile based_FOW](Images/Tile-Based Fog of War.png)

In this approach, we will keep track of all the tiles in the map, to know what must be done at each tile in terms of visuals and logic, specially regarding entities such as enemies or structures.

There are a couple of ways to do this, but generally we have a 2D container with the same size as our map. In the container, we will store ID’s referring to the state of each tile in its correspondent position, to know if it’s unvisited, covered in fog or completely clear (visible). With this approach we can also have metadata stored in another equal in size 2D container in order to create zones with special conditions, such as always visible areas, areas only visible for certain entities, bushes that hide everything under them until you are inside the bush etc.

The main drawback of this approach is that as tiles have edges, we will need to think and code a solution to smooth the edges of the tiles.



