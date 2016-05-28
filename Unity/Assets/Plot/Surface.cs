using UnityEngine;

public class Surface
{
	private int x = 0;
	private int y = 0;

	private float[,] heightMap;

	private float xyScale = 1f;
	private float heightScale = 1f;
	private float maxHeight = 0f;

	public Mesh mesh;

	public Surface(int xLength, int yLength) {
		x = xLength;
		y = yLength;
		heightMap = new float[x, y];
	}

	public void SetData(float[] data)
	{
		heightMap = new float[x, y];
		int n = 0;
		for (int i = 0; i < x; i++) {
			for (int j = 0; j < y; j++) {
				heightMap[i, j] = data[n++];
			}
		}
		maxHeight = 0f;
	}

	public void SetData(float[,] data)
	{
		heightMap = data;
		maxHeight = 0f;
	}
	
	public void SetXYScale(float scale) {
		xyScale = scale;
	}

	public float GetXYScale() {
		return xyScale;
	}

	public void SetHeighScale(float scale) {
		heightScale = scale;
	}
	
	public float GetHeighScale() {
		return heightScale;
	}

	public float this[int id] {
		get { return heightMap[id / x, id % y]; }
	}

	public float this[int x, int y] {
		get { return heightMap[x, y]; }
	}
	
	public Vector3 GetPoint(int id) {
		return new Vector3(id / x * xyScale, heightMap[id / x, id % y] * heightScale, (id % y) * xyScale);
	}

	public Vector3 GetPoint(int x, int y) {
		return new Vector3(x * xyScale, heightMap [x, y] * heightScale, y * xyScale);
	}
	
	public int GetLengthX() {
		return x;
	}

	public int GetLengthY() {
		return y;
	}

	public int GetLength() {
		return heightMap.Length;
	}
	
	public float GetMaxHeight()
	{
		if (maxHeight == 0f)
		{
			float max = heightMap[0, 0];
			float min = heightMap[0, 0];
			for (int i = 0; i < x; i++)
			{
				for (int j = 0; j < y; j++)
				{
					float val = heightMap[i, j];
					if (val > max) {
						max = val;
					}
					if (val < min) {
						min = val;
					}
				}
			}
			maxHeight = (max - min) * heightScale;
		}
		return maxHeight;
	}

	public float[,] AsTwoDimensional() {
		return heightMap;
	}
}