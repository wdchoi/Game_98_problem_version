#pragma once

#include <QtCore/QObject>

#include <Qt3DCore/qentity.h>
#include <Qt3DCore/qtransform.h>

#include <Qt3DRender/QTorusMesh>
#include <Qt3DRender/QCylinderMesh>
#include <Qt3DRender/QCuboidMesh>
#include <Qt3DRender/QSphereMesh>
#include <Qt3DRender/QPhongMaterial>
#include <QTimer>

//#include "Utilities\RandomNumberGenerator.h"

#include "NeuralNetwork.h"
#include "Environment.h"

class SceneModifier : public QObject
{
	Q_OBJECT

public:
	explicit SceneModifier(Qt3DCore::QEntity *rootEntity);
	~SceneModifier();

    QTimer *idle_timer_;
    //RandomNumberGenerator rand_;
    NeuralNetwork nn_;
    Environment env_;

public slots:
	void enableCuboid(bool enabled);
    void update();
    void updateSubstep(const bool training);

private:
	Qt3DCore::QEntity *m_rootEntity;
	//Qt3DCore::QEntity *m_cuboidEntity;

    Qt3DCore::QEntity *agent_entity_;
    Qt3DCore::QEntity *target_entity_;
    
public:
    //Qt3DCore::QTransform *cuboidTransform;

    Qt3DCore::QTransform *agent_transform_;
    Qt3DCore::QTransform *target_transform_;

    void moveTarget();

    void moveObjectsFromHistory();   

//protected:
//	virtual void keyPressEvent(QKeyEvent *e);
};
