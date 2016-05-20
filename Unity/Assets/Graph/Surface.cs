using UnityEngine;

public class Surface
{
	public int x = 4;
	public int y = 4;
	
//	public float[,] heightMap = { 	
//		{1f, 0f, 0f, 0f},
//		{0f, 2f, 0f, 0f},
//		{0f, 0f, 0f, 0f},
//		{0f, 0f, 0f, 0f}};

	public float[,] heightMap = { 	
		{0f, 0f, 0f, 0f},
		{0f, 0f, 0f, 0f},
		{0f, 0f, 2f, 0f},
		{0f, 0f, 0f, 1f}};
	
	public Mesh mesh;
	
	private float maxHeight = 0f;
	
	public float GetHeight(int id) {
		return heightMap[id / x, id % y];
	}
	public float GetHeight(int x, int y) {
		return heightMap[x, y];
	}
	
	public Vector3 GetPoint(int id) {
		return new Vector3(id / x, id % y, heightMap[id / x, id % y]);
	}

	public Vector3 GetPoint(int x, int y) {
		return new Vector3(x, y, heightMap [x, y]);
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
			maxHeight = max - min;
			Debug.Log(maxHeight);
		}
		return maxHeight;
	}
}
