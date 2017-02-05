/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "scenemodifier.h"

#include <Qt3DRender/QMesh>
#include <Qt3DRender/qabstractattribute.h>
#include <iostream>
#include <QtCore/QDebug>

#include "Vector2D.h"

SceneModifier::SceneModifier(Qt3DCore::QEntity *rootEntity)
	: env_(2),
	m_rootEntity(rootEntity), idle_timer_(new QTimer(this))
{
	env_.initialize();

	nn_.initialize((1 + 2) * env_.history_.array_.num_elements_, 3, 1, 1);    // input: ag_p_x, ball_x/z, output: left, stay, right

	nn_.layer_type_act_[0] = 2;
	nn_.layer_type_act_[1] = 2;
	nn_.layer_type_act_[2] = 2;

	nn_.eta_ = 1e-4;
	nn_.alpha_ = 0.5;

	Qt3DRender::QPhongMaterial *red_material = new Qt3DRender::QPhongMaterial();
	red_material->setDiffuse(QColor(QRgb(0xFF0000)));

	Qt3DRender::QPhongMaterial *blue_material = new Qt3DRender::QPhongMaterial();
	blue_material->setDiffuse(QColor(QRgb(0x8470ff)));

	target_entity_ = new Qt3DCore::QEntity(m_rootEntity);
	agent_entity_ = new Qt3DCore::QEntity(m_rootEntity);

	target_transform_ = new Qt3DCore::QTransform();
	target_transform_->setScale(0.02f);
	target_transform_->setTranslation(QVector3D(env_.ball_pos_.x_, env_.ball_pos_.y_, env_.ball_pos_.z_));

	agent_transform_ = new Qt3DCore::QTransform();
	agent_transform_->setScale3D(QVector3D(env_.agent_width_*1.0, 0.05, 0.05));
	agent_transform_->setTranslation(QVector3D(env_.agent_pos_.x_, env_.agent_pos_.y_, env_.agent_pos_.z_));

	target_entity_->addComponent(target_transform_);
	target_entity_->addComponent(red_material);
	target_entity_->addComponent(new Qt3DRender::QSphereMesh());

	agent_entity_->addComponent(agent_transform_);
	agent_entity_->addComponent(blue_material);
	agent_entity_->addComponent(new Qt3DRender::QCuboidMesh());

	// training
	for (int pre = 0; pre < 1000; pre++)
	{
		for (int r = 0; r < 100000; r++)
			updateSubstep(true);

		std::cout << "Training " << pre / 1.0 << " %" << std::endl;
	}

	std::cout << "Training ended." << std::endl;

	// check performance
	for (int r = 0; r < 100000; r++)
		updateSubstep(false);

	QObject::connect(idle_timer_, SIGNAL(timeout()), SLOT(update()));

	idle_timer_->setInterval(15);
	idle_timer_->start();
}

SceneModifier::~SceneModifier()
{
}

void SceneModifier::enableCuboid(bool enabled)
{
}

void SceneModifier::update()
{
	
	idle_timer_->stop();

	updateSubstep(false);

	idle_timer_->start();
}

void SceneModifier::updateSubstep(const bool is_training)
{
	VectorND<D> input;
	input.Initialize(nn_.num_input_);

	int count = 0;
	for (int h = 0; h < env_.num_records_; h++)
	{
		GameState gs;
		gs = env_.history_.getValue(h);

		input[count] = gs.agent_pos_.x_;
		input[count + 1] = gs.ball_pos_.x_;
		input[count + 2] = gs.ball_pos_.z_;

		count += 3;
	}

	nn_.setInputVector(input);
	nn_.feedForward();

	int selected_dir;

	if (is_training == true)
	{
		// epsilon greedy
		const T epsilon = 0.1;

		if ((T)rand()/RAND_MAX < epsilon)
		{
			// try random dir
			selected_dir = rand() % 3;
		}
		else
		{
			selected_dir = nn_.getIXMaxCompOutput();
		}
	}
	else
		selected_dir = nn_.getIXMaxCompOutput();

	const double dt = env_.agent_width_*0.33;

	switch (selected_dir)
	{
	case 0:
		env_.updateAgent(0, 0, 0, dt);
		break;
	case 1:
		env_.updateAgent(-1.0, 0, 0, dt);
		break;
	case 2:
		env_.updateAgent(1.0, 0, 0, dt);
		break;
	default:
		std::cout << "Wrong direction" << std::endl;
	}

	D success_rate = 0.0;

	const D reward = env_.update(0.04, success_rate) - 0.0000;

	if (is_training)
	{
		VectorND<D> reward_vector;
		nn_.copyOutputVector(reward_vector, false);

		D next_Q;
		{
			VectorND<D> next_input;
			next_input.Initialize(nn_.num_input_);

			int count = 0;
			for (int h = 0; h < env_.num_records_; h++)
			{
				GameState gs;
				gs = env_.history_.getValue(h);

				next_input[count] = gs.agent_pos_.x_;
				next_input[count + 1] = gs.ball_pos_.x_;
				next_input[count + 2] = gs.ball_pos_.z_;

				count += 3;
			}

			nn_.setInputVector(next_input);

			nn_.feedForward();

			next_Q = nn_.getValueMaxCompOutput();

		}

		const T gamma = 0.95;

		for (int d = 0; d < reward_vector.num_dimension_; d++)
		{
			if (selected_dir == d)
			{
				reward_vector[d] = reward + gamma * next_Q;
			}
		}

		nn_.setInputVector(input);
		nn_.feedForward();
		nn_.propBackward(reward_vector);
	}

	moveTarget();
}

void SceneModifier::moveTarget()
{
	target_transform_->setTranslation(QVector3D(env_.ball_pos_.x_, env_.ball_pos_.y_, env_.ball_pos_.z_));
	agent_transform_->setTranslation(QVector3D(env_.agent_pos_.x_, env_.agent_pos_.y_, env_.agent_pos_.z_));
}

void SceneModifier::moveObjectsFromHistory()
{
	static int step;

	GameState st = env_.history_.getValue(step++);

	target_transform_->setTranslation(QVector3D(st.ball_pos_.x_, st.ball_pos_.y_, st.ball_pos_.z_));
	agent_transform_->setTranslation(QVector3D(st.agent_pos_.x_, st.agent_pos_.y_, st.agent_pos_.z_));
}