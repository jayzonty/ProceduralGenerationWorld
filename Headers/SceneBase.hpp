#pragma once

/// <summary>
/// Scene base class
/// </summary>
class SceneBase
{
private:

public:
	/// <summary>
	/// Constructor
	/// </summary>
	SceneBase()
	{
	}

	/// <summary>
	/// Destructor
	/// </summary>
	virtual ~SceneBase()
	{
	}

	/// <summary>
	/// Start scene
	/// </summary>
	virtual void Start() = 0;

	/// <summary>
	/// Finish scene
	/// </summary>
	virtual void Finish() = 0;

	/// <summary>
	/// Updates scene state
	/// </summary>
	/// <param name="deltaTime"></param>
	virtual void Update(float deltaTime) = 0;

	/// <summary>
	/// Draws the scene
	/// </summary>
	virtual void Draw() = 0;
};
