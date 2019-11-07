#include "main.h"

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
pros::vision_signature_s_t sig = pros::Vision::signature_from_utility(1, 273, 2061, 1167, -5189, -4777, -4983, 5.200, 0);


pros::vision_signature_s_t sig2 = pros::Vision::signature_from_utility(1, 8369, 8775, 8572, -1729, -1391, -1560, 11.000, 0);

pros::Controller controller(pros::E_CONTROLLER_MASTER);

pros::Motor frontRight(1, pros::E_MOTOR_GEARSET_18, true);
pros::Motor frontLeft(11, pros::E_MOTOR_GEARSET_18, false);

pros::Motor backRight(10, pros::E_MOTOR_GEARSET_18, true);
pros::Motor backLeft(9, pros::E_MOTOR_GEARSET_18, false);

pros::Vision vs(1, pros::E_VISION_ZERO_CENTER);

void opcontrol()
{
	frontRight.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	frontLeft.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);

	backRight.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	backLeft.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);

	vs.clear_led();



	vs.set_signature(1, &sig2);
	LV_IMG_DECLARE(ball);
	//extern const lv_img_t ball;
	lv_obj_t * im = lv_img_create(lv_scr_act(), NULL);

	lv_img_set_src(im, &ball);
	lv_obj_set_pos(im, 0, 0);


 lv_obj_t * xoffL =  lv_label_create(lv_scr_act(), NULL);
 lv_obj_align(xoffL, NULL, LV_ALIGN_CENTER, 100, -75);

 lv_obj_t * wid =  lv_label_create(lv_scr_act(), NULL);
 lv_obj_align(wid, NULL, LV_ALIGN_CENTER, 100, -50);

 lv_obj_t * state =  lv_label_create(lv_scr_act(), NULL);
 lv_obj_align(state, NULL, LV_ALIGN_CENTER, 100, -25);

 lv_obj_t * name =  lv_label_create(lv_scr_act(), NULL);
 lv_obj_align(name, NULL, LV_ALIGN_CENTER, 100, 0);

 lv_obj_t * name2 =  lv_label_create(lv_scr_act(), NULL);
 lv_obj_align(name2, NULL, LV_ALIGN_CENTER, 100, 20);

 lv_label_set_text(name, "Programmed by");
 lv_label_set_text(name2, "Tad Keller");

	while(true)
	{
		vs.clear_led();

		pros::vision_object_s_t val = vs.get_by_sig(0, 1);

		std::string xoff = "X Offset : " + std::to_string(val.x_middle_coord);
		std::string widV = "Cube Width : " + std::to_string(val.width);

		lv_label_set_text(xoffL, xoff.c_str());
		lv_label_set_text(wid, widV.c_str());

		if((val.x_middle_coord > 70 || val.x_middle_coord < -70) && !(val.x_middle_coord < -300 || val.x_middle_coord > 300))
		{
			if(val.x_middle_coord > 70)
			{
				lv_label_set_text(state, "turning");
				pros::delay(10);
				backLeft.move(50);
				frontLeft.move(50);

				backRight.move(-50);
				frontRight.move(-50);
			}
			else
			if(val.x_middle_coord < -70)
			{
				lv_label_set_text(state, "turning");
				pros::delay(10);
				backLeft.move(-50);
				frontLeft.move(-50);

				backRight.move(50);
				frontRight.move(50);
			}
			vs.set_led(COLOR_ORANGE);

			continue;
		}
		else
		{
			lv_obj_set_pos(im, 30, val.x_middle_coord * 1);

		if(val.width < 100 && val.width != 0)
		{
			lv_label_set_text(state, "forward");
			backLeft.move(90);
			frontLeft.move(90);

			backRight.move(90);
			frontRight.move(90);

			vs.set_led(COLOR_ORANGE);
		}
	}
	if((!(val.width < 100 && val.width != 0) && !(val.x_middle_coord > 70 || val.x_middle_coord < -70)) || (val.x_middle_coord < -300 || val.x_middle_coord > 300))
	{
			lv_label_set_text(state, "idle");

			backLeft.move_velocity(0);
			frontLeft.move_velocity(0);

			backRight.move_velocity(0);
			frontRight.move_velocity(0);

			vs.set_led(COLOR_GREEN);

		}
		if(val.width == 0 && (val.x_middle_coord == 0 || val.x_middle_coord == -31371))
		{
			lv_label_set_text(state, "searching");
			pros::delay(10);
			backLeft.move(50);
			frontLeft.move(50);

			backRight.move(-50);
			frontRight.move(-50);

			vs.set_led(COLOR_ORANGE);
		}
		pros::delay(20);
	}
}
