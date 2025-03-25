//============================================================
//
//	判定関数処理 [collision.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "collision.h"

//============================================================
//	XY平面の矩形の当たり判定
//============================================================
bool collision::BoxXY
(
	const VECTOR3& rCenterPos,		// 判定位置
	const VECTOR3& rTargetPos,		// 判定目標位置
	const VECTOR3& rCenterSizeUp,	// 判定大きさ (右/上/後)
	const VECTOR3& rCenterSizeDown,	// 判定大きさ (左/下/前)
	const VECTOR3& rTargetSizeUp,	// 判定目標大きさ (右/上/後)
	const VECTOR3& rTargetSizeDown	// 判定目標大きさ (左/下/前)
)
{
	if (rCenterPos.x + rCenterSizeUp.x   > rTargetPos.x - rTargetSizeDown.x
	&&  rCenterPos.y + rCenterSizeUp.y   > rTargetPos.y - rTargetSizeDown.y
	&&  rCenterPos.x - rCenterSizeDown.x < rTargetPos.x + rTargetSizeUp.x
	&&  rCenterPos.y - rCenterSizeDown.y < rTargetPos.y + rTargetSizeUp.y)
	{ // 判定内の場合

		return true;
	}

	return false;
}

//============================================================
//	XZ平面の矩形の当たり判定
//============================================================
bool collision::BoxXZ
(
	const VECTOR3& rCenterPos,		// 判定位置
	const VECTOR3& rTargetPos,		// 判定目標位置
	const VECTOR3& rCenterSizeUp,	// 判定大きさ (右/上/後)
	const VECTOR3& rCenterSizeDown,	// 判定大きさ (左/下/前)
	const VECTOR3& rTargetSizeUp,	// 判定目標大きさ (右/上/後)
	const VECTOR3& rTargetSizeDown	// 判定目標大きさ (左/下/前)
)
{
	if (rCenterPos.x + rCenterSizeUp.x   > rTargetPos.x - rTargetSizeDown.x
	&&  rCenterPos.z + rCenterSizeUp.z   > rTargetPos.z - rTargetSizeDown.z
	&&  rCenterPos.x - rCenterSizeDown.x < rTargetPos.x + rTargetSizeUp.x
	&&  rCenterPos.z - rCenterSizeDown.z < rTargetPos.z + rTargetSizeUp.z)
	{ // 判定内の場合

		return true;
	}

	return false;
}

//============================================================
//	3軸の矩形の当たり判定
//============================================================
bool collision::Box3D
(
	VECTOR3 centerPos,		// 判定位置
	VECTOR3 targetPos,		// 判定目標位置
	VECTOR3 centerSizeUp,	// 判定サイズ(右・上・後)
	VECTOR3 centerSizeDown,	// 判定サイズ(左・下・前)
	VECTOR3 targetSizeUp,	// 判定目標サイズ(右・上・後)
	VECTOR3 targetSizeDown	// 判定目標サイズ(左・下・前)
)
{
	if (centerPos.x + centerSizeUp.x   > targetPos.x - targetSizeDown.x
	&&  centerPos.y + centerSizeUp.y   > targetPos.y - targetSizeDown.y
	&&  centerPos.z + centerSizeUp.z   > targetPos.z - targetSizeDown.z
	&&  centerPos.x - centerSizeDown.x < targetPos.x + targetSizeUp.x
	&&  centerPos.y - centerSizeDown.y < targetPos.y + targetSizeUp.y
	&&  centerPos.z - centerSizeDown.z < targetPos.z + targetSizeUp.z)
	{ // 判定内の場合

		return true;
	}

	return false;
}

//============================================================
//	XZ平面の円の当たり判定
//============================================================
bool collision::Circle2D
(
	VECTOR3 centerPos,		// 判定位置
	VECTOR3 targetPos,		// 判定目標位置
	float fCenterRadius,	// 判定半径
	float fTargetRadius,	// 判定目標半径
	float* pLength			// 判定目標との距離
)
{
	// 判定位置と判定目標位置の距離を求める
	float fLength = (centerPos.x - targetPos.x) * (centerPos.x - targetPos.x)
				  + (centerPos.z - targetPos.z) * (centerPos.z - targetPos.z);

	if (pLength != nullptr)
	{ // ポインタが使用されている場合

		// 判定目標との距離を代入
		*pLength = fLength;
	}

	if (fLength < ((fCenterRadius + fTargetRadius) * (fCenterRadius + fTargetRadius)))
	{ // 判定内の場合

		return true;
	}

	return false;
}

//============================================================
//	3軸の円の当たり判定
//============================================================
bool collision::Circle3D
(
	VECTOR3 centerPos,		// 判定位置
	VECTOR3 targetPos,		// 判定目標位置
	float fCenterRadius,	// 判定半径
	float fTargetRadius		// 判定目標半径
)
{
	// 判定位置と判定目標位置の距離を求める
	float fLength = (centerPos.x - targetPos.x) * (centerPos.x - targetPos.x)
				  + (centerPos.y - targetPos.y) * (centerPos.y - targetPos.y)
				  + (centerPos.z - targetPos.z) * (centerPos.z - targetPos.z);

	if (fLength < ((fCenterRadius + fTargetRadius) * (fCenterRadius + fTargetRadius)))
	{ // 判定内の場合

		return true;
	}

	return false;
}

//============================================================
//	XZ平面の扇形の当たり判定
//============================================================
bool collision::Sector
(
	VECTOR3 centerPos,	// 判定位置
	VECTOR3 targetPos,	// 判定目標位置
	float fCenterRot,	// 判定向き
	float fRadius,		// 視界範囲
	float fAngle,		// 視野角
	float* pLength		// 判定目標との距離
)
{
	bool bHit = false;	// 当たり判定の結果

	// 中心位置と目標位置の距離求める
	float fLength = (centerPos.x - targetPos.x) * (centerPos.x - targetPos.x)
				  + (centerPos.z - targetPos.z) * (centerPos.z - targetPos.z);

	if (pLength != nullptr)
	{ // ポインタが使用されている場合

		// 判定目標との距離を代入
		*pLength = fLength;
	}

	if (fLength < fRadius * fRadius)
	{ // 円の範囲内の場合

		float fRotEdge[2];	// 扇形の縁の角度     [※] 0：左 1：右
		VECTOR3 posEdge[2];	// 扇形の縁の先端位置 [※] 0：左 1：右
		VECTOR3 vecEdge[2];	// 扇形の縁ベクトル   [※] 0：左 1：右

		// 引数の角度の半分の値を求める
		float fHalfAngle = fAngle * 0.5f;

		// 扇形の左縁の角度を求める
		fRotEdge[0] = fCenterRot + fHalfAngle;	// 角度を左に傾ける
		useful::NormalizeRot(fRotEdge[0]);		// 向きを正規化

		// 扇形の右縁の角度を求める
		fRotEdge[1] = fCenterRot - fHalfAngle;	// 角度を右に傾ける
		useful::NormalizeRot(fRotEdge[1]);		// 向きを正規化

		// 扇形の左縁の先端位置を求める
		posEdge[0].x = centerPos.x + sinf(fRotEdge[0]);
		posEdge[0].y = 0.0f;
		posEdge[0].z = centerPos.z + cosf(fRotEdge[0]);

		// 扇形の右縁の先端位置を求める
		posEdge[1].x = centerPos.x + sinf(fRotEdge[1]);
		posEdge[1].y = 0.0f;
		posEdge[1].z = centerPos.z + cosf(fRotEdge[1]);

		// 扇形の左縁のベクトルを求める
		vecEdge[0] = posEdge[0] - centerPos;
		vecEdge[0].y = 0.0f;

		// 扇形の右縁のベクトルを求める
		vecEdge[1] = posEdge[1] - centerPos;
		vecEdge[1].y = 0.0f;

		// 左端と位置のベクトルを求める
		VECTOR3 vecToPos = targetPos - centerPos;

		if ((vecEdge[0].z * vecToPos.x) - (vecEdge[0].x * vecToPos.z) < 0
		&&  (vecEdge[1].z * vecToPos.x) - (vecEdge[1].x * vecToPos.z) > 0)
		{ // 扇形の両縁の範囲内の場合

			// 当たっている状態にする
			bHit = true;
		}
	}

	// 当たり判定の結果を返す
	return bHit;
}

//============================================================
//	XY平面の角柱の衝突判定
//============================================================
bool collision::ResponseBoxPillarXY
(
	VECTOR3& rCenterPos,			// 判定位置
	const VECTOR3& rCenterPosOld,	// 判定過去位置
	const VECTOR3& rTargetPos,		// 判定目標位置
	const VECTOR3& rCenterSizeUp,	// 判定大きさ (右/上/後)
	const VECTOR3& rCenterSizeDown,	// 判定大きさ (左/下/前)
	const VECTOR3& rTargetSizeUp,	// 判定目標大きさ (右/上/後)
	const VECTOR3& rTargetSizeDown	// 判定目標大きさ (左/下/前)
)
{
	bool bHit = false;	// 衝突判定結果

	// 左右の当たり判定
	if (rCenterPos.y + rCenterSizeUp.y > rTargetPos.y - rTargetSizeDown.y
	&&  rCenterPos.y - rCenterSizeDown.y < rTargetPos.y + rTargetSizeUp.y)
	{ // 前後の範囲内の場合

		if (rCenterPos.x    + rCenterSizeUp.x >  rTargetPos.x - rTargetSizeDown.x
		&&  rCenterPosOld.x + rCenterSizeUp.x <= rTargetPos.x - rTargetSizeDown.x)
		{ // 左からの当たり判定

			// 位置を補正
			rCenterPos.x = rTargetPos.x - rTargetSizeDown.x - rCenterSizeUp.x;

			// 衝突状態にする
			bHit = true;
		}
		else if (rCenterPos.x    - rCenterSizeDown.x <  rTargetPos.x + rTargetSizeUp.x
		     &&  rCenterPosOld.x - rCenterSizeDown.x >= rTargetPos.x + rTargetSizeUp.x)
		{ // 右からの当たり判定
			
			// 位置を補正
			rCenterPos.x = rTargetPos.x + rTargetSizeUp.x + rCenterSizeDown.x;

			// 衝突状態にする
			bHit = true;
		}
	}

	// 上下の当たり判定
	if (rCenterPos.x + rCenterSizeUp.x > rTargetPos.x - rTargetSizeDown.x
	&&  rCenterPos.x - rCenterSizeDown.x < rTargetPos.x + rTargetSizeUp.x)
	{ // 左右の範囲内の場合

		if (rCenterPos.y    + rCenterSizeUp.y >  rTargetPos.y - rTargetSizeDown.y
		&&  rCenterPosOld.y + rCenterSizeUp.y <= rTargetPos.y - rTargetSizeDown.y)
		{ // 前からの当たり判定

			// 位置を補正
			rCenterPos.y = rTargetPos.y - rTargetSizeDown.y - rCenterSizeUp.y;

			// 衝突状態にする
			bHit = true;
		}
		else if (rCenterPos.y    - rCenterSizeDown.y <  rTargetPos.y + rTargetSizeUp.y
		     &&  rCenterPosOld.y - rCenterSizeDown.y >= rTargetPos.y + rTargetSizeUp.y)
		{ // 後からの当たり判定

			// 位置を補正
			rCenterPos.y = rTargetPos.y + rTargetSizeUp.y + rCenterSizeDown.y;

			// 衝突状態にする
			bHit = true;
		}
	}

	// 衝突判定を返す
	return bHit;
}

//============================================================
//	XZ平面の角柱の衝突判定
//============================================================
bool collision::ResponseBoxPillarXZ
(
	VECTOR3& rCenterPos,			// 判定位置
	const VECTOR3& rCenterPosOld,	// 判定過去位置
	const VECTOR3& rTargetPos,		// 判定目標位置
	const VECTOR3& rCenterSizeUp,	// 判定大きさ (右/上/後)
	const VECTOR3& rCenterSizeDown,	// 判定大きさ (左/下/前)
	const VECTOR3& rTargetSizeUp,	// 判定目標大きさ (右/上/後)
	const VECTOR3& rTargetSizeDown	// 判定目標大きさ (左/下/前)
)
{
	bool bHit = false;	// 衝突判定結果

	// 前後の当たり判定
	if (rCenterPos.x + rCenterSizeUp.x > rTargetPos.x - rTargetSizeDown.x
	&&  rCenterPos.x - rCenterSizeDown.x < rTargetPos.x + rTargetSizeUp.x)
	{ // 左右の範囲内の場合

		if (rCenterPos.z    + rCenterSizeUp.z >  rTargetPos.z - rTargetSizeDown.z
		&&  rCenterPosOld.z + rCenterSizeUp.z <= rTargetPos.z - rTargetSizeDown.z)
		{ // 前からの当たり判定

			// 位置を補正
			rCenterPos.z = rTargetPos.z - rTargetSizeDown.z - rCenterSizeUp.z;

			// 衝突状態にする
			bHit = true;
		}
		else if (rCenterPos.z    - rCenterSizeDown.z <  rTargetPos.z + rTargetSizeUp.z
		     &&  rCenterPosOld.z - rCenterSizeDown.z >= rTargetPos.z + rTargetSizeUp.z)
		{ // 後からの当たり判定

			// 位置を補正
			rCenterPos.z = rTargetPos.z + rTargetSizeUp.z + rCenterSizeDown.z;

			// 衝突状態にする
			bHit = true;
		}
	}

	// 左右の当たり判定
	if (rCenterPos.z + rCenterSizeUp.z > rTargetPos.z - rTargetSizeDown.z
	&&  rCenterPos.z - rCenterSizeDown.z < rTargetPos.z + rTargetSizeUp.z)
	{ // 前後の範囲内の場合

		if (rCenterPos.x    + rCenterSizeUp.x >  rTargetPos.x - rTargetSizeDown.x
		&&  rCenterPosOld.x + rCenterSizeUp.x <= rTargetPos.x - rTargetSizeDown.x)
		{ // 左からの当たり判定

			// 位置を補正
			rCenterPos.x = rTargetPos.x - rTargetSizeDown.x - rCenterSizeUp.x;

			// 衝突状態にする
			bHit = true;
		}
		else if (rCenterPos.x    - rCenterSizeDown.x <  rTargetPos.x + rTargetSizeUp.x
		     &&  rCenterPosOld.x - rCenterSizeDown.x >= rTargetPos.x + rTargetSizeUp.x)
		{ // 右からの当たり判定
			
			// 位置を補正
			rCenterPos.x = rTargetPos.x + rTargetSizeUp.x + rCenterSizeDown.x;

			// 衝突状態にする
			bHit = true;
		}
	}

	// 衝突判定を返す
	return bHit;
}

//============================================================
//	円柱の衝突判定
//============================================================
bool collision::CirclePillar
(
	VECTOR3& rCenterPos,	// 判定位置
	VECTOR3 targetPos,		// 判定目標位置
	float fCenterRadius,	// 判定半径
	float fTargetRadius		// 判定目標半径
)
{
	// 判定位置と判定目標位置の距離を求める
	float fLength = (rCenterPos.x - targetPos.x) * (rCenterPos.x - targetPos.x)
				  + (rCenterPos.z - targetPos.z) * (rCenterPos.z - targetPos.z);

	if (fLength < (fCenterRadius + fTargetRadius) * (fCenterRadius + fTargetRadius))
	{ // 判定内の場合

		// 位置を補正
		float fCenterRot = atan2f(rCenterPos.x - targetPos.x, rCenterPos.z - targetPos.z);	// 判定目標から見た判定向き
		rCenterPos.x = targetPos.x + sinf(fCenterRot) * (fCenterRadius + fTargetRadius);
		rCenterPos.z = targetPos.z + cosf(fCenterRot) * (fCenterRadius + fTargetRadius);

		return true;
	}

	return false;
}

//============================================================
//	三軸の矩形の衝突判定
//============================================================
bool collision::ResponseBox3D
(
	VECTOR3& rCenterPos,			// 判定位置
	const VECTOR3& rCenterPosOld,	// 判定過去位置
	const VECTOR3& rTargetPos,		// 判定目標位置
	const VECTOR3& rCenterSizeUp,	// 判定サイズ(右・上・後)
	const VECTOR3& rCenterSizeDown,	// 判定サイズ(左・下・前)
	const VECTOR3& rTargetSizeUp,	// 判定目標サイズ(右・上・後)
	const VECTOR3& rTargetSizeDown,	// 判定目標サイズ(左・下・前)
	VECTOR3* pMove,	// 移動量
	bool* pUp,		// 上からの判定
	bool* pSide,	// 横からの判定
	bool* pDown		// 下からの判定
)
{
	// 左右の当たり判定
	bool bHit = false;	// 衝突判定結果
	if (rCenterPos.y + rCenterSizeUp.y   > rTargetPos.y - rTargetSizeDown.y
	&&  rCenterPos.y - rCenterSizeDown.y < rTargetPos.y + rTargetSizeUp.y
	&&  rCenterPos.z + rCenterSizeUp.z   > rTargetPos.z - rTargetSizeDown.z
	&&  rCenterPos.z - rCenterSizeDown.z < rTargetPos.z + rTargetSizeUp.z)
	{ // 上下と前後の範囲内の場合

		if (rCenterPos.x    + rCenterSizeUp.x >  rTargetPos.x - rTargetSizeDown.x
		&&  rCenterPosOld.x + rCenterSizeUp.x <= rTargetPos.x - rTargetSizeDown.x)
		{ // 左からの当たり判定

			// 位置を補正
			rCenterPos.x = rTargetPos.x - rTargetSizeDown.x - rCenterSizeUp.x;

			// 衝突状態にする
			bHit = true;

			if (pMove != nullptr)
			{ // ポインタが使用されている場合

				// 移動量を初期化
				pMove->x = 0.0f;
			}

			if (pSide != nullptr)
			{ // ポインタが使用されている場合

				// 横に当たっている状態を設定
				*pSide = true;
			}
		}
		else if (rCenterPos.x    - rCenterSizeDown.x <  rTargetPos.x + rTargetSizeUp.x
		     &&  rCenterPosOld.x - rCenterSizeDown.x >= rTargetPos.x + rTargetSizeUp.x)
		{ // 右からの当たり判定
			
			// 位置を補正
			rCenterPos.x = rTargetPos.x + rTargetSizeUp.x + rCenterSizeDown.x;

			// 衝突状態にする
			bHit = true;

			if (pMove != nullptr)
			{ // ポインタが使用されている場合

				// 移動量を初期化
				pMove->x = 0.0f;
			}

			if (pSide != nullptr)
			{ // ポインタが使用されている場合

				// 横に当たっている状態を設定
				*pSide = true;
			}
		}
	}

	// 上下の当たり判定
	if (rCenterPos.x + rCenterSizeUp.x   > rTargetPos.x - rTargetSizeDown.x
	&&  rCenterPos.x - rCenterSizeDown.x < rTargetPos.x + rTargetSizeUp.x
	&&  rCenterPos.z + rCenterSizeUp.z   > rTargetPos.z - rTargetSizeDown.z
	&&  rCenterPos.z - rCenterSizeDown.z < rTargetPos.z + rTargetSizeUp.z)
	{ // 左右と前後の範囲内の場合

		if (rCenterPos.y    + rCenterSizeUp.y >  rTargetPos.y - rTargetSizeDown.y
		&&  rCenterPosOld.y + rCenterSizeUp.y <= rTargetPos.y - rTargetSizeDown.y)
		{ // 下からの当たり判定

			// 位置を補正
			rCenterPos.y = rTargetPos.y - rTargetSizeDown.y - rCenterSizeUp.y;

			// 衝突状態にする
			bHit = true;

			if (pMove != nullptr)
			{ // ポインタが使用されている場合

				// 移動量を初期化
				pMove->y = 0.0f;
			}

			if (pDown != nullptr)
			{ // ポインタが使用されている場合

				// 下に当たっている状態を設定
				*pDown = true;
			}
		}
		else if (rCenterPos.y    - rCenterSizeDown.y <  rTargetPos.y + rTargetSizeUp.y
		     &&  rCenterPosOld.y - rCenterSizeDown.y >= rTargetPos.y + rTargetSizeUp.y)
		{ // 上からの当たり判定
			
			// 位置を補正
			rCenterPos.y = rTargetPos.y + rTargetSizeUp.y + rCenterSizeDown.y;

			// 衝突状態にする
			bHit = true;

			if (pMove != nullptr)
			{ // ポインタが使用されている場合

				// 移動量を初期化
				pMove->y = 0.0f;
			}

			if (pUp != nullptr)
			{ // ポインタが使用されている場合

				// 上に当たっている状態を設定
				*pUp = true;
			}
		}
	}

	// 前後の当たり判定
	if (rCenterPos.x + rCenterSizeUp.x   > rTargetPos.x - rTargetSizeDown.x
	&&  rCenterPos.x - rCenterSizeDown.x < rTargetPos.x + rTargetSizeUp.x
	&&  rCenterPos.y + rCenterSizeUp.y   > rTargetPos.y - rTargetSizeDown.y
	&&  rCenterPos.y - rCenterSizeDown.y < rTargetPos.y + rTargetSizeUp.y)
	{ // 左右と上下の範囲内の場合

		if (rCenterPos.z    + rCenterSizeUp.z >  rTargetPos.z - rTargetSizeDown.z
		&&  rCenterPosOld.z + rCenterSizeUp.z <= rTargetPos.z - rTargetSizeDown.z)
		{ // 前からの当たり判定

			// 位置を補正
			rCenterPos.z = rTargetPos.z - rTargetSizeDown.z - rCenterSizeUp.z;

			// 衝突状態にする
			bHit = true;

			if (pMove != nullptr)
			{ // ポインタが使用されている場合

				// 移動量を初期化
				pMove->z = 0.0f;
			}

			if (pSide != nullptr)
			{ // ポインタが使用されている場合

				// 横に当たっている状態を設定
				*pSide = true;
			}
		}
		else if (rCenterPos.z    - rCenterSizeDown.z <  rTargetPos.z + rTargetSizeUp.z
		     &&  rCenterPosOld.z - rCenterSizeDown.z >= rTargetPos.z + rTargetSizeUp.z)
		{ // 後からの当たり判定

			// 位置を補正
			rCenterPos.z = rTargetPos.z + rTargetSizeUp.z + rCenterSizeDown.z;

			// 衝突状態にする
			bHit = true;

			if (pMove != nullptr)
			{ // ポインタが使用されている場合

				// 移動量を初期化
				pMove->z = 0.0f;
			}

			if (pSide != nullptr)
			{ // ポインタが使用されている場合

				// 横に当たっている状態を設定
				*pSide = true;
			}
		}
	}

	// 衝突判定を返す
	return bHit;
}

//============================================================
//	三軸の円の衝突判定
//============================================================
bool collision::ResponseCircle3D
(
	VECTOR3& rCenterPos,	// 判定位置
	VECTOR3 targetPos,		// 判定目標位置
	float fCenterRadius,	// 判定半径
	float fTargetRadius		// 判定目標半径
)
{
	// 判定位置と判定目標位置の距離を求める
	float fLength = (rCenterPos.x - targetPos.x) * (rCenterPos.x - targetPos.x)
				  + (rCenterPos.y - targetPos.y) * (rCenterPos.y - targetPos.y)
				  + (rCenterPos.z - targetPos.z) * (rCenterPos.z - targetPos.z);

	if (fLength < ((fCenterRadius + fTargetRadius) * (fCenterRadius + fTargetRadius)))
	{ // 判定内の場合

		VECTOR3 vecRev = rCenterPos - targetPos;	// 補正方向
		VECTOR3 sphere;	// 球座標
		float fPhi, fTheta;	// 方位角・仰角

		// ベクトルを向きに変換し、向きから球座標を求める
		useful::VecToRot(vecRev, &fPhi, &fTheta);
		useful::RotToVec(fPhi, fTheta, &sphere);

		// 位置を補正
		rCenterPos = targetPos + sphere * (fCenterRadius + fTargetRadius);

		return true;
	}

	return false;
}

//============================================================
//	三軸のカプセルの衝突判定
//============================================================
bool collision::ResponseCapsule3D
(
	VECTOR3* pCenterPos,	// 判定位置
	VECTOR3& rTargetPos,	// 判定目標位置
	float fCenterRadius,	// 判定半径
	float fTargetRadius,	// 判定目標半径
	float fTargetHeight		// 判定目標縦幅
)
{
	bool bHit = false;	// 衝突判定結果
	float fHalfHeight = (fTargetHeight - (fTargetRadius * 2.0f)) * 0.5f;	// 円柱縦幅の半分
	VECTOR3 centUp   = *pCenterPos + VECTOR3(0.0f, fCenterRadius, 0.0f);	// 判定位置の上
	VECTOR3 centDown = *pCenterPos - VECTOR3(0.0f, fCenterRadius, 0.0f);	// 判定位置の下
	VECTOR3 targUp   = rTargetPos + VECTOR3(0.0f, fHalfHeight, 0.0f);		// 判定目標位置の上
	VECTOR3 targDown = rTargetPos - VECTOR3(0.0f, fHalfHeight, 0.0f);		// 判定目標位置の下
	if (centDown.y >= targUp.y)
	{ // 円柱判定の上側にいる場合

		// 上の球判定
		bHit = collision::ResponseCircle3D
		( // 引数
			*pCenterPos,	// 判定位置
			targUp,			// 判定目標位置
			fCenterRadius,	// 判定半径
			fTargetRadius	// 判定目標半径
		);
	}
	else if (centUp.y <= targDown.y)
	{ // 円柱判定の下側にいる場合

		// 下の球判定
		bHit = collision::ResponseCircle3D
		( // 引数
			*pCenterPos,	// 判定位置
			targDown,		// 判定目標位置
			fCenterRadius,	// 判定半径
			fTargetRadius	// 判定目標半径
		);
	}
	else
	{ // 上下の範囲内の場合

		// 円柱の判定
		bHit = collision::CirclePillar
		( // 引数
			*pCenterPos,	// 判定位置
			rTargetPos,		// 判定目標位置
			fCenterRadius,	// 判定半径
			fTargetRadius	// 判定目標半径
		);
	}

	// 衝突判定を返す
	return bHit;
}

//============================================================
//	X軸の衝突判定
//============================================================
bool collision::ResponseSingleX
(
	VECTOR3& rCenterPos,	// 判定位置
	VECTOR3& rCenterPosOld,	// 判定過去位置
	VECTOR3 targetPos,		// 判定目標位置
	VECTOR3 centerSizeUp,	// 判定サイズ(右・上・後)
	VECTOR3 centerSizeDown,	// 判定サイズ(左・下・前)
	VECTOR3 targetSizeUp,	// 判定目標サイズ(右・上・後)
	VECTOR3 targetSizeDown,	// 判定目標サイズ(左・下・前)
	VECTOR3* pMove,	// 移動量
	bool* pLeft,	// 左からの判定
	bool* pRight	// 右からの判定
)
{
	// 左右の当たり判定
	bool bHit = false;	// 衝突判定結果
	if (rCenterPos.y + centerSizeUp.y   > targetPos.y - targetSizeDown.y
	&&  rCenterPos.y - centerSizeDown.y < targetPos.y + targetSizeUp.y
	&&  rCenterPos.z + centerSizeUp.z   > targetPos.z - targetSizeDown.z
	&&  rCenterPos.z - centerSizeDown.z < targetPos.z + targetSizeUp.z)
	{ // 上下と前後の範囲内の場合

		if (rCenterPos.x    + centerSizeUp.x >  targetPos.x - targetSizeDown.x
		&&  rCenterPosOld.x + centerSizeUp.x <= targetPos.x - targetSizeDown.x)
		{ // 左からの当たり判定

			// 位置を補正
			rCenterPos.x = targetPos.x - targetSizeDown.x - centerSizeUp.x;

			// 衝突状態にする
			bHit = true;

			if (pMove != nullptr)
			{ // ポインタが使用されている場合

				// 移動量を初期化
				pMove->x = 0.0f;
			}

			if (pLeft != nullptr)
			{ // ポインタが使用されている場合

				// 左に当たっている状態を設定
				*pLeft = true;
			}
		}
		else if (rCenterPos.x    - centerSizeDown.x <  targetPos.x + targetSizeUp.x
		     &&  rCenterPosOld.x - centerSizeDown.x >= targetPos.x + targetSizeUp.x)
		{ // 右からの当たり判定
			
			// 位置を補正
			rCenterPos.x = targetPos.x + targetSizeUp.x + centerSizeDown.x;

			// 衝突状態にする
			bHit = true;

			if (pMove != nullptr)
			{ // ポインタが使用されている場合

				// 移動量を初期化
				pMove->x = 0.0f;
			}

			if (pRight != nullptr)
			{ // ポインタが使用されている場合

				// 右に当たっている状態を設定
				*pRight = true;
			}
		}
	}

	// 衝突判定を返す
	return bHit;
}

//============================================================
//	Y軸の衝突判定
//============================================================
bool collision::ResponseSingleY
(
	VECTOR3& rCenterPos,	// 判定位置
	VECTOR3& rCenterPosOld,	// 判定過去位置
	VECTOR3 targetPos,		// 判定目標位置
	VECTOR3 centerSizeUp,	// 判定サイズ(右・上・後)
	VECTOR3 centerSizeDown,	// 判定サイズ(左・下・前)
	VECTOR3 targetSizeUp,	// 判定目標サイズ(右・上・後)
	VECTOR3 targetSizeDown,	// 判定目標サイズ(左・下・前)
	VECTOR3* pMove,	// 移動量
	bool* pDown,	// 下からの判定
	bool* pUp		// 上からの判定
)
{
	// 上下の当たり判定
	bool bHit = false;	// 衝突判定結果
	if (rCenterPos.x + centerSizeUp.x   > targetPos.x - targetSizeDown.x
	&&  rCenterPos.x - centerSizeDown.x < targetPos.x + targetSizeUp.x
	&&  rCenterPos.z + centerSizeUp.z   > targetPos.z - targetSizeDown.z
	&&  rCenterPos.z - centerSizeDown.z < targetPos.z + targetSizeUp.z)
	{ // 左右と前後の範囲内の場合

		if (rCenterPos.y    + centerSizeUp.y >  targetPos.y - targetSizeDown.y
		&&  rCenterPosOld.y + centerSizeUp.y <= targetPos.y - targetSizeDown.y)
		{ // 下からの当たり判定

			// 位置を補正
			rCenterPos.y = targetPos.y - targetSizeDown.y - centerSizeUp.y;

			// 衝突状態にする
			bHit = true;

			if (pMove != nullptr)
			{ // ポインタが使用されている場合

				// 移動量を初期化
				pMove->y = 0.0f;
			}

			if (pDown != nullptr)
			{ // ポインタが使用されている場合

				// 下に当たっている状態を設定
				*pDown = true;
			}
		}
		else if (rCenterPos.y    - centerSizeDown.y <  targetPos.y + targetSizeUp.y
		     &&  rCenterPosOld.y - centerSizeDown.y >= targetPos.y + targetSizeUp.y)
		{ // 上からの当たり判定
			
			// 位置を補正
			rCenterPos.y = targetPos.y + targetSizeUp.y + centerSizeDown.y;

			// 衝突状態にする
			bHit = true;

			if (pMove != nullptr)
			{ // ポインタが使用されている場合

				// 移動量を初期化
				pMove->y = 0.0f;
			}

			if (pUp != nullptr)
			{ // ポインタが使用されている場合

				// 上に当たっている状態を設定
				*pUp = true;
			}
		}
	}

	// 衝突判定を返す
	return bHit;
}

//============================================================
//	Z軸の衝突判定
//============================================================
bool collision::ResponseSingleZ
(
	VECTOR3& rCenterPos,	// 判定位置
	VECTOR3& rCenterPosOld,	// 判定過去位置
	VECTOR3 targetPos,		// 判定目標位置
	VECTOR3 centerSizeUp,	// 判定サイズ(右・上・後)
	VECTOR3 centerSizeDown,	// 判定サイズ(左・下・前)
	VECTOR3 targetSizeUp,	// 判定目標サイズ(右・上・後)
	VECTOR3 targetSizeDown,	// 判定目標サイズ(左・下・前)
	VECTOR3* pMove,	// 移動量
	bool* pBefore,	// 前からの判定
	bool* pAfter	// 後からの判定
)
{
	// 前後の当たり判定
	bool bHit = false;	// 衝突判定結果
	if (rCenterPos.x + centerSizeUp.x   > targetPos.x - targetSizeDown.x
	&&  rCenterPos.x - centerSizeDown.x < targetPos.x + targetSizeUp.x
	&&  rCenterPos.y + centerSizeUp.y   > targetPos.y - targetSizeDown.y
	&&  rCenterPos.y - centerSizeDown.y < targetPos.y + targetSizeUp.y)
	{ // 左右と上下の範囲内の場合

		if (rCenterPos.z    + centerSizeUp.z >  targetPos.z - targetSizeDown.z
		&&  rCenterPosOld.z + centerSizeUp.z <= targetPos.z - targetSizeDown.z)
		{ // 前からの当たり判定

			// 位置を補正
			rCenterPos.z = targetPos.z - targetSizeDown.z - centerSizeUp.z;

			// 衝突状態にする
			bHit = true;

			if (pMove != nullptr)
			{ // ポインタが使用されている場合

				// 移動量を初期化
				pMove->z = 0.0f;
			}

			if (pBefore != nullptr)
			{ // ポインタが使用されている場合

				// 前に当たっている状態を設定
				*pBefore = true;
			}
		}
		else if (rCenterPos.z    - centerSizeDown.z <  targetPos.z + targetSizeUp.z
		     &&  rCenterPosOld.z - centerSizeDown.z >= targetPos.z + targetSizeUp.z)
		{ // 後からの当たり判定

			// 位置を補正
			rCenterPos.z = targetPos.z + targetSizeUp.z + centerSizeDown.z;

			// 衝突状態にする
			bHit = true;

			if (pMove != nullptr)
			{ // ポインタが使用されている場合

				// 移動量を初期化
				pMove->z = 0.0f;
			}

			if (pAfter != nullptr)
			{ // ポインタが使用されている場合

				// 後に当たっている状態を設定
				*pAfter = true;
			}
		}
	}

	// 衝突判定を返す
	return bHit;
}

//============================================================
//	XY平面角柱の内側制限
//============================================================
bool collision::InBoxPillarXY
(
	VECTOR3& rCenterPos,			// 判定位置
	const VECTOR3& rOriginPos,		// 判定原点位置
	const VECTOR3& rCenterSizeUp,	// 判定サイズ(右・上・後)
	const VECTOR3& rCenterSizeDown,	// 判定サイズ(左・下・前)
	const VECTOR3& rOriginSizeUp,	// 判定原点サイズ(右・上・後)
	const VECTOR3& rOriginSizeDown	// 判定原点サイズ(左・下・前)
)
{
	bool bHit = false;	// 判定結果

	// 左右の補正
	if (useful::LimitNum(rCenterPos.x, rOriginPos.x - rOriginSizeDown.x + rCenterSizeUp.x, rOriginPos.x + rOriginSizeUp.x - rCenterSizeDown.x))
	{ // 補正が行われた場合

		// 判定した状態にする
		bHit = true;
	}

	// 上下の補正
	if (useful::LimitNum(rCenterPos.y, rOriginPos.y - rOriginSizeDown.y + rCenterSizeUp.y, rOriginPos.y + rOriginSizeUp.y - rCenterSizeDown.y))
	{ // 補正が行われた場合

		// 判定した状態にする
		bHit = true;
	}

	// 判定結果を返す
	return bHit;
}

//============================================================
//	XZ平面角柱の内側制限
//============================================================
bool collision::InBoxPillarXZ
(
	VECTOR3& rCenterPos,			// 判定位置
	const VECTOR3& rOriginPos,		// 判定原点位置
	const VECTOR3& rCenterSizeUp,	// 判定サイズ(右・上・後)
	const VECTOR3& rCenterSizeDown,	// 判定サイズ(左・下・前)
	const VECTOR3& rOriginSizeUp,	// 判定原点サイズ(右・上・後)
	const VECTOR3& rOriginSizeDown	// 判定原点サイズ(左・下・前)
)
{
	bool bHit = false;	// 判定結果

	// 左右の補正
	if (useful::LimitNum(rCenterPos.x, rOriginPos.x - rOriginSizeDown.x + rCenterSizeUp.x, rOriginPos.x + rOriginSizeUp.x - rCenterSizeDown.x))
	{ // 補正が行われた場合

		// 判定した状態にする
		bHit = true;
	}

	// 前後の補正
	if (useful::LimitNum(rCenterPos.z, rOriginPos.z - rOriginSizeDown.z + rCenterSizeUp.z, rOriginPos.z + rOriginSizeUp.z - rCenterSizeDown.z))
	{ // 補正が行われた場合

		// 判定した状態にする
		bHit = true;
	}

	// 判定結果を返す
	return bHit;
}

//============================================================
//	円柱の内側制限
//============================================================
bool collision::InCirclePillar
(
	VECTOR3& rCenterPos,	// 判定位置
	VECTOR3 originPos,		// 判定原点位置
	float fCenterRadius,	// 判定半径
	float fOriginRadius		// 判定原点半径
)
{
	// 判定位置と判定目標位置の距離を求める
	float fLength = (rCenterPos.x - originPos.x) * (rCenterPos.x - originPos.x)
				  + (rCenterPos.z - originPos.z) * (rCenterPos.z - originPos.z);

	if (fLength > (fOriginRadius - fCenterRadius) * (fOriginRadius - fCenterRadius))
	{ // 判定外の場合

		// 位置を補正
		float fCenterRot = atan2f(rCenterPos.x - originPos.x, rCenterPos.z - originPos.z);	// 判定目標から見た判定向き
		rCenterPos.x = originPos.x + sinf(fCenterRot) * (fOriginRadius - fCenterRadius);
		rCenterPos.z = originPos.z + cosf(fCenterRot) * (fOriginRadius - fCenterRadius);

		return true;
	}

	return false;
}

//============================================================
//	外積の左右判定
//============================================================
//	境界線から見て左右どちらにいるかの判定に使用
//============================================================
float collision::LineOuterProduct
(
	VECTOR3 posLeft,	// 境界線左座標
	VECTOR3 posRight,	// 境界線右座標
	VECTOR3 pos			// 判定位置
)
{
	// 境界線ベクトルを求める
	VECTOR3 vecLine = posRight - posLeft;

	// 左端と位置のベクトルを求める
	VECTOR3 vecToPos = pos - posLeft;

	// 外積の計算結果を返す
	return (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);
}

//============================================================
//	外積の上下判定
//============================================================
//	境界線から見て上下どちらにいるかの判定に使用
//============================================================
float collision::LineHeightOuterProduct
(
	VECTOR3 posLeft,	// 境界線左座標
	VECTOR3 posRight,	// 境界線右座標
	VECTOR3 pos			// 判定位置
)
{
	// 境界線ベクトルを求める
	VECTOR3 vecLine = posRight - posLeft;

	// 左端と位置のベクトルを求める
	VECTOR3 vecToPos = pos - posLeft;

	// 外積の計算結果を返す
	return (vecLine.y * vecToPos.x) - (vecLine.x * vecToPos.y);
}

//============================================================
//	三角柱の当たり判定
//============================================================
bool collision::TriangleOuterPillar
(
	VECTOR3 pos0,	// 三角の各頂点
	VECTOR3 pos1,	// 三角の各頂点
	VECTOR3 pos2,	// 三角の各頂点
	VECTOR3 pos		// 判定位置
)
{
	if (collision::LineOuterProduct(pos0, pos1, pos) <= 0.0f
	&&  collision::LineOuterProduct(pos1, pos2, pos) <= 0.0f
	&&  collision::LineOuterProduct(pos2, pos0, pos) <= 0.0f)
	{ // 判定内の場合

		return true;
	}

	return false;
}

//============================================================
//	四角柱の当たり判定
//============================================================
bool collision::BoxOuterPillar
(
	VECTOR3 pos0,	// 四角の各頂点
	VECTOR3 pos1,	// 四角の各頂点
	VECTOR3 pos2,	// 四角の各頂点
	VECTOR3 pos3,	// 四角の各頂点
	VECTOR3 pos		// 判定位置
)
{
	if (collision::LineOuterProduct(pos0, pos1, pos) <= 0.0f
	&&  collision::LineOuterProduct(pos1, pos2, pos) <= 0.0f
	&&  collision::LineOuterProduct(pos2, pos3, pos) <= 0.0f
	&&  collision::LineOuterProduct(pos3, pos0, pos) <= 0.0f)
	{ // 判定内の場合

		return true;
	}

	return false;
}
