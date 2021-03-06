/*
@project: PK Creator
@author: Piotr Krupa
*/

var canvas = document.getElementById('canvas');
var ctx = canvas.getContext('2d');

var allObjects = [];
var idxCurrentScene;
var allScenes = [];
var allFonts = [];

// CurrentScene object
var currentScene;

var currentId = 0;

// Common
function GetID()
{
	currentId++;
	return currentId;
}

function Font(id, family, size, color, align)
{
	this.id = id;
	this.family = family;
	this.size = size;
	this.color = color;
	this.align = align;

	this.draw = function() {
		 ctx.font = this.size + 'px ' + this.family;
		 ctx.fillStyle = this.color;
		 ctx.textAlign = this.align;
	}
}

function AddFont(font)
{
	allFonts.push(font);
}

function GetFont(fontID)
{
	for (var i = 0; i < allFonts.length; i++)
	{
		if (allFonts[i].id == fontID)
		{
			return allFonts[i];
		}
	}
}

// Sprite
function Sprite(imagePath, centerX, centerY)
{
	this.img = new Image();
	this.img.src = imagePath;

	this.centerX = centerX;
	this.centerY = centerY;
}

function CollisionEvent(objectID, func)
{
	this.objectID = objectID;
	this.func = func;
}

// Object
function Object(id, sprite, solid, visible)
{
	this.id = id;
	this.sprite = sprite;
	this.vars = [];

	this.events = [];

	this.collisions = [];
	this.hspeed = 0;
	this.vspeed = 0;

	this.solid = solid;
	this.visible = visible;

	// 0 - none
	// 1 - left
	// 2 - right
	this.collisionHor = 0;

	// 0 - none
	// 1 - top
	// 2 - bottom 
	this.collsionVer = 0;

	this.addCollisionListener = function(objectID, func) {
		var collEvent = new CollisionEvent(objectID, func);
		this.collisions.push(collEvent);
	}

	this.draw = function() {
		
		if (visible)
		{
			var rad = (this.angle) * Math.PI / 180;
			
			ctx.save();
			ctx.translate(this.x, this.y);
			ctx.rotate(rad);
			ctx.drawImage(this.sprite.img, -this.sprite.centerX, -this.sprite.centerY, this.sprite.img.width, this.sprite.img.height);
			ctx.restore();
		}

		if (this.events.renderEvent)
			this.events.renderEvent(this.id);
	}

	this.pulse = function() {
		if (this.hspeed > 0 && this.collisionHor == 2 ||
			this.hspeed < 0 && this.collisionHor == 1)
		{
			this.hspeed = 0;	
		}

		if (this.vspeed > 0 && this.collsionVer == 2 ||
			this.vspeed < 0 && this.collsionVer == 1)
		{
			this.vspeed = 0;	
		}

		this.x += this.hspeed;
		this.y += this.vspeed;

		var thisX = this.x - this.sprite.centerX;
		var thisY = this.y - this.sprite.centerY;

		for (var i = 0; i < this.collisions.length; i++)
		{
			var coll = this.collisions[i];

			var instances = GetInstancesByObjectID(coll.objectID);

			var breakIt = false;

			for (var j = 0; j < instances.length; j++)
			{
				var instance = instances[j];

				var instanceX = instance.x - instance.sprite.centerX;
				var instanceY = instance.y - instance.sprite.centerY;

				// left && right
				if (this.sprite.img.height < instance.sprite.img.height)
				{
					// left collide with right
					if (thisX + this.hspeed + this.sprite.img.width > instanceX && thisX + this.sprite.img.width <= instanceX + instance.sprite.img.width / 2 &&
					(
						(thisY > instanceY &&
						 thisY < instanceY + instance.sprite.img.height) ||
						(thisY + this.sprite.img.height > instanceY &&
						 thisY + this.sprite.img.height < instanceY + instance.sprite.img.height)
					))
					{
						this.collisionHor = 2;
					}
					// right collide with left
					else if (thisX + this.hspeed <= instanceX + instance.sprite.img.width && thisX >= instanceX - instance.sprite.img.width / 2 &&
					(
						(thisY > instanceY &&
						 thisY < instanceY + instance.sprite.img.height) ||
						(thisY + this.sprite.img.height > instanceY &&
						 thisY + this.sprite.img.height < instanceY + instance.sprite.img.height)
					))
					{
						this.collisionHor = 1;
					}
					else
					{
						this.collisionHor = 0;
					}
				}
				else if (this.sprite.img.height > instance.sprite.img.height)
				{
					// left collide with right
					if (thisX + this.hspeed + this.sprite.img.width > instanceX && thisX + this.sprite.img.width <= instanceX + instance.sprite.img.width / 2 &&
					(
						(thisY < instanceY &&
						 thisY + this.sprite.img.height > instanceY) ||
						(thisY < instanceY + instance.sprite.img.height &&
						 thisY + this.sprite.img.height > instanceY + instance.sprite.img.height)
					))
					{
						this.collisionHor = 2;
					}
					// right collide with left
					else if (thisX + this.hspeed <= instanceX + instance.sprite.img.width && thisX >= instanceX - instance.sprite.img.width / 2 &&
					(
						(thisY < instanceY &&
						 thisY + this.sprite.img.height > instanceY) ||
						(thisY < instanceY + instance.sprite.img.height &&
						 thisY + this.sprite.img.height > instanceY + instance.sprite.img.height)
					))
					{
						this.collisionHor = 1;
					}
					else
					{
						this.collisionHor = 0;
					}
				}

				// top && bottom
				if (this.sprite.img.width < instance.sprite.img.width)
				{
					// bottom collide to top
					if (thisY + this.vspeed + this.sprite.img.height >= instanceY && thisY + this.sprite.img.height <= instanceY + instance.sprite.img.height / 2 &&
					(
							(thisX > instanceX &&
							 thisX < instanceX + instance.sprite.img.width) ||
							(thisX + this.sprite.img.width > instanceX &&
							 thisX + this.sprite.img.width < instanceX + instance.sprite.img.width)
						))
						{
							this.collsionVer = 2;
						}
					// top collide to bottom
					else if (thisY + this.vspeed <= instanceY + instance.sprite.img.height && thisY >= instanceY + instance.sprite.img.height / 2 &&
					(
							(thisX > instanceX &&
							 thisX < instanceX + instance.sprite.img.width) ||
							(thisX + this.sprite.img.width > instanceX &&
							 thisX + this.sprite.img.width < instanceX + instance.sprite.img.width)
						))
						{
							this.collsionVer = 1;
						}
					else
					{
						this.collsionVer = 0;
					}
				}
				else if (this.sprite.img.width > instance.sprite.img.width)
				{
					// bottom collide to top
					if (thisY + this.sprite.img.height >= instanceY && thisY + this.sprite.img.height <= instanceY + instance.sprite.img.height / 2 &&
					(
							(thisX < instanceX &&
							 thisX + this.sprite.img.width > instanceX) ||
							(thisX < instanceX + instance.sprite.img.width &&
							 thisX + this.sprite.img.width > instanceX + instance.sprite.img.width)
						))
						{
							this.collsionVer = 2;
						}
					// top collide to bottom
					else if (thisY + this.vspeed <= instanceY + instance.sprite.img.height && thisY >= instanceY + instance.sprite.img.height / 2 &&
					(
							(thisX < instanceX &&
							 thisX + this.sprite.img.width > instanceX) ||
							(thisX < instanceX + instance.sprite.img.width &&
							 thisX + this.sprite.img.width > instanceX + instance.sprite.img.width)
						))
						{
							this.collsionVer = 1;
						}
					else
					{
						this.collsionVer = 0;
					}
				}

				if (this.collisionHor > 0 || this.collsionVer > 0)
				{
					breakIt = true;
					break;
				}
			}

			if (breakIt)
			{
				if (this.collisionHor > 0)
				{
					coll.func(this.id, instance.id, this.collisionHor)
				}
				else if (this.collsionVer > 0)
				{
					coll.func(this.id, instance.id, this.collsionVer + 2);
				}

				if (!instance.solid)
				{
					this.collisionHor = 0;
					this.collsionVer = 0;
				}

				break;
			}
		}
		if (this.events.pulseEvent)
			this.events.pulseEvent(this.id);
	}
}

function AddObject(object)
{
	allObjects.push(object);
}

function GetObjectById(objectID)
{
	for (var i = 0; i < allObjects.length; i++)
	{
		if (allObjects[i].id == allObjects)
			return allObjects[i];
	}
}

// CurrentScene && Instance
function CurrentScene(scene)
{
	this.bgColor = scene.bgColor;
	this.scene = scene;

	this.instances = [];
}

function Instance(object, x, y)
{
	this.id = GetID();
	this.object = object;
	this.sprite = object.sprite;
	this.vars = object.vars;
	this.events = object.events;

	this.collisions = object.collisions;

	this.solid = object.solid;
	this.visible = object.visible;

	this.y = y;
	this.x = x;

	this.hspeed = 0;
	this.vspeed = 0;

	this.angle = 0;

	this.draw = object.draw;
	this.pulse = object.pulse;
}

function CreateInstance(objectID, x, y)
{
	for (var i = 0; i < allObjects.length; ++i)
	{
		if (allObjects[i].id == objectID)
		{
			var instance = new Instance(allObjects[i], x, y);

			currentScene.instances.push(instance);

			if (instance.events.createEvent)
				instance.events.createEvent(instance.id);

			return instance.id;
		}
	}
}

function DestroyInstance(instanceID)
{
	console.log("Destroying " + instanceID);

	for (var i = 0; i < currentScene.instances.length; ++i)
	{
		var instance = currentScene.instances[i];
		if (instance.id == instanceID)
		{
			if (instance.events.destroyEvent)
				instance.events.destroyEvent(instance.id);

			currentScene.instances.splice(i, 1);
		}
	}
}

function GetInstancesByObjectID(objectID)
{
	var instances = [];

	for (var i = 0; i < currentScene.instances.length; ++i)
	{
		var instance = currentScene.instances[i];

		if (instance.object.id == objectID)
		{
			instances.push(instance);
		}
	}

	return instances;
}

function GetInstanceByInstanceID(instanceID, x, y)
{
	for (var i = 0; i < currentScene.instances.length; i++)
	{
		var instance = currentScene.instances[i];

		if (instance.id == instanceID)
		{
			return instance;
		}
	}
}

// Scene
function Scene(id, bgColor, bgImagePath, bgTile)
{
	this.id = id;
	this.bgColor = bgColor;
	this.bgImage = new Image();
	this.bgImage.src = bgImagePath;
	this.bgTile = bgTile;

	this.sceneObjects = [];

	this.draw = function() 
	{ 
		var maxX = 0;
		var maxY = 0;

		if (bgTile == 3 || bgTile == 1)
		{
			if (this.bgImage.width)
				maxX = canvas.width / this.bgImage.width;
		}

		if (bgTile == 3 || bgTile == 2)
		{
			if (this.bgImage.height)
				maxY = canvas.height / this.bgImage.height;
		}

		for (var i = 0; i <= maxX; i++)
		{
			for (var j = 0; j <= maxY; j++)
			{
				ctx.drawImage(this.bgImage, this.bgImage.width * i, this.bgImage.height * j);
			}
		}


	}
}

function SceneObject(objectID, x, y)
{
	this.objectID = objectID;
	this.x = x;
	this.y = y;
}

function AddScene(scene)
{
	allScenes.push(scene);
}

function UnloadCurrentScene()
{
	if (currentScene)
		currentScene.instances = [];
}

function LoadScene(sceneID)
{
	UnloadCurrentScene();

	for (var i = 0; i < allScenes.length; i++)
	{
		if (allScenes[i].id == sceneID)
		{
			idxCurrentScene = i;
			currentScene = new CurrentScene(allScenes[i]);

			for (var j = 0; j < allScenes[i].sceneObjects.length; j++)
			{
				var sceneObject = allScenes[i].sceneObjects[j];

				CreateInstance(sceneObject.objectID, sceneObject.x, sceneObject.y);
			}
		}
	}
}



// Events
canvas.onmousemove = function(data) { 
	var x = data.x;
	var y = data.y;

	for (i = 0; i < currentScene.instances.length; i++)
	{
		var instance = currentScene.instances[i];

		if (instance.events.mouseMovedEvent)
			instance.events.mouseMovedEvent(instance.id,x,y);
	}
}

// Mouse pressed
canvas.onmousedown = function(data) { 
	var button = data.button;
	var x = data.x;
	var y = data.y;

	for (i = 0; i < currentScene.instances.length; i++)
	{
		var instance = currentScene.instances[i];

		if (instance.events.mouseDownEvent)
			instance.events.mouseDownEvent(instance.id,x,y,button);
	}
}

// Mouse released
canvas.onmouseup = function(data) { 
	var button = data.button;
	var x = data.x;
	var y = data.y;

	for (i = 0; i < currentScene.instances.length; i++)
	{	
		var instance = currentScene.instances[i];

		if (instance.events.mouseUpEvent)
			instance.events.mouseUpEvent(instance.id,x,y,button);	
	}
}

window.onkeydown = function(data) { 
	var key = data.keyCode;

	for (i = 0; i < currentScene.instances.length; i++)
	{
		var instance = currentScene.instances[i];

		if (instance.events.keyDownEvent)
			instance.events.keyDownEvent(instance.id,key);
	}
}

window.onkeyup = function(data) { 
	var key = data.keyCode;

	for (i = 0; i < currentScene.instances.length; i++)
	{	
		var instance = currentScene.instances[i];

		if (instance.events.keyUpEvent)
			instance.events.keyUpEvent(instance.id,key);
	}
}
