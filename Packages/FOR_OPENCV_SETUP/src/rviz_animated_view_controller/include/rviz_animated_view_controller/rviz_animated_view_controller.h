/*
 * Copyright (c) 2012, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Author: Adam Leeper
 */

#ifndef RVIZ_ANIMATED_VIEW_CONTROLLER_H
#define RVIZ_ANIMATED_VIEW_CONTROLLER_H

#include <boost/circular_buffer.hpp>

#include <cv_bridge/cv_bridge.h>

#include <image_transport/image_transport.h>

#include "rviz/view_controller.h"
#include "rviz/view_manager.h"
#include "rviz/render_panel.h"

#include <ros/subscriber.h>
#include <ros/ros.h>

#include <geometry_msgs/Pose.h>

#include <std_msgs/Bool.h>
#include <std_msgs/Duration.h>

#include <view_controller_msgs/CameraMovement.h>
#include <view_controller_msgs/CameraPlacement.h>
#include <view_controller_msgs/CameraTrajectory.h>

#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreVector3.h>
#include <OGRE/OgreQuaternion.h>

namespace rviz {
  class SceneNode;
  class Shape;
  class BoolProperty;
  class FloatProperty;
  class VectorProperty;
  class QuaternionProperty;
  class TfFrameProperty;
  class EditableEnumProperty;
  class RosTopicProperty;
}

namespace rviz_animated_view_controller
{

/** @brief An un-constrained "flying" camera, specified by an eye point, focus point, and up vector. */
class AnimatedViewController : public rviz::ViewController
{
Q_OBJECT
public:

  enum { TRANSITION_LINEAR = 0,
         TRANSITION_SPHERICAL};

  struct OgreCameraMovement
  {
    OgreCameraMovement(){};

    OgreCameraMovement(const Ogre::Vector3& eye,
                       const Ogre::Vector3& focus,
                       const Ogre::Vector3& up,
                       const ros::Duration& transition_duration,
                       const uint8_t interpolation_speed)
      : eye(eye)
        , focus(focus)
        , up(up)
        , transition_duration(transition_duration)
        , interpolation_speed(interpolation_speed)
    {
    }

    Ogre::Vector3 eye;
    Ogre::Vector3 focus;
    Ogre::Vector3 up;

    ros::Duration transition_duration;
    uint8_t interpolation_speed;
  };

  typedef boost::circular_buffer<OgreCameraMovement> BufferCamMovements;

  AnimatedViewController();
  virtual ~AnimatedViewController();

  void initializePublishers();
  void initializeSubscribers();

  /** @brief Do subclass-specific initialization.  Called by
   * ViewController::initialize after context_ and camera_ are set.
   *
   * This version sets up the attached_scene_node, focus shape, and subscribers. */
  virtual void onInitialize();

  /** @brief called by activate().
   *
   * This version calls updateAttachedSceneNode(). */
  virtual void onActivate();


  /** @brief Applies a translation to the focus and eye points. */
  void move_focus_and_eye( float x, float y, float z );

  /** @brief Applies a translation to only the eye point. */
  void move_eye( float x, float y, float z );


  /** @brief Applies a body-fixed-axes sequence of rotations;
      only accurate for small angles. */
  void yaw_pitch_roll( float yaw, float pitch, float roll );


  virtual void handleMouseEvent(rviz::ViewportMouseEvent& evt);

  /** @brief Publishes the current camera pose. */
  void publishCameraPose();
  
  /** @brief Calls beginNewTransition() to
      move the focus point to the point provided, assumed to be in the Rviz Fixed Frame */
  virtual void lookAt( const Ogre::Vector3& point );


  /** @brief Calls beginNewTransition() with the focus point fixed, moving the eye to the point given. */
  void orbitCameraTo( const Ogre::Vector3& point);

  /** @brief Calls beginNewTransition() to move the eye to the point given, keeping the direction fixed.*/
  void moveEyeWithFocusTo( const Ogre::Vector3& point);

  /** @brief Resets the camera parameters to a sane value. */
  virtual void reset();

  /** @brief Configure the settings of this view controller to give,
   * as much as possible, a similar view as that given by the
   * @a source_view.
   *
   * @a source_view must return a valid @c Ogre::Camera* from getCamera(). */
  virtual void mimic( ViewController* source_view );

  /** @brief Called by ViewManager when this ViewController is being made current.
   * @param previous_view is the previous "current" view, and will not be NULL.
   *
   * This gives ViewController subclasses an opportunity to implement
   * a smooth transition from a previous viewpoint to the new
   * viewpoint.
   */
  virtual void transitionFrom( ViewController* previous_view );


protected Q_SLOTS:
  /** @brief Called when Target Frame property changes while view is
   * active.  Purpose is to change values in the view controller (like
   * a position offset) such that the actual viewpoint does not
   * change.  Calls updateTargetSceneNode() and
   * onTargetFrameChanged(). */
  virtual void updateAttachedFrame();
  
  /** @brief Called when distance property is changed; computes new eye position. */
  virtual void onDistancePropertyChanged();

  /** @brief Called focus property is changed; computes new distance. */
  virtual void onFocusPropertyChanged();
  
  /** @brief Called when eye property is changed; computes new distance. */
  virtual void onEyePropertyChanged();

  /** @brief Called when up vector property is changed (does nothing for now...). */
  virtual void onUpPropertyChanged();

protected:  //methods
  void updateWindowSizeProperties();

  /** @brief Called at 30Hz by ViewManager::update() while this view
   * is active. Override with code that needs to run repeatedly. */
  virtual void update(float dt, float ros_dt);

  /** @brief Pauses the animation if pause_animation_duration_ is larger than zero.
   *
   * Adds the pause_animation_duration_ to the transition_start_time_ to continue the animation from
   * where it was paused.
   */
  void pauseAnimationOnRequest();

  /** @brief Returns true if buffer contains at least one start and end pose needed for one movement. */
  bool isMovementAvailable(){ return cam_movements_buffer_.size() >= 2; };

  /** @brief Computes the fraction of time we already used for the current movement. 
   * 
   * If we are rendering frame by frame we compute the passed time counting the frames we already rendered.
   * Dividing by the total number of frames we want to render for the current transition results in the progress.
   * 
   * @params[in] transition_duration    total duration of the current movement.
   * 
   * @returns Relative progress in time as a float between 0 and 1.
   */
  double computeRelativeProgressInTime(const ros::Duration& transition_duration);
  
  /** @brief Convert the relative progress in time to the corresponding relative progress in space wrt. the interpolation speed profile.
   *
   * Example: 
   *   The camera has to move from point A to point B in a duration D. 
   *   The camera should accelerate slowly and arrive at full speed - RISING speed profile. 
   *   At exactly half of the duration D the camera wouldn't be right in the middle between A and B, because it needed 
   *   time to accelerate. 
   *   This method converts the relative progress in time specified by a number between zero and one, to the relative
   *   progress in space as a number between zero and one. 
   *   
   * Interpolation speed profiles:
   * RISING    = 0 # Speed of the camera rises smoothly - resembles the first quarter of a sinus wave.
   * DECLINING = 1 # Speed of the camera declines smoothly - resembles the second quarter of a sinus wave.
   * FULL      = 2 # Camera is always at full speed - depending on transition_duration.
   * WAVE      = 3 # RISING and DECLINING concatenated in one movement.
   * 
   * @params[in] relative_progress_in_time  the relative progress in time, between 0 and 1.
   * @params[in] interpolation_speed        speed profile.
   * 
   * @returns relative progress in space as a float between 0 and 1.
   */
  float computeRelativeProgressInSpace(double relative_progress_in_time,
                                       uint8_t interpolation_speed);

  /** @brief Publish the rendered image that is visible to the user in rviz. */
  void publishViewImage();

  /** @brief Get the current image rviz is showing as an Ogre::PixelBox. */
  void getViewImage(std::shared_ptr<Ogre::PixelBox>& pixel_box);

  void convertImage(std::shared_ptr<Ogre::PixelBox> input_image,
                    sensor_msgs::ImagePtr output_image);
  
  /** @brief Updates the transition_start_time_ and resets the rendered_frames_counter_ for next movement. */
  void prepareNextMovement(const ros::Duration& previous_transition_duration);
  
  /** @brief Convenience function; connects the signals/slots for position properties. */
  void connectPositionProperties();

  /** @brief Convenience function; disconnects the signals/slots for position properties. */
  void disconnectPositionProperties();

  /** @brief Override to implement the change in properties which
   * nullifies the change in attached frame.
   * @see updateAttachedFrame() */
  virtual void onAttachedFrameChanged( const Ogre::Vector3& old_reference_position, const Ogre::Quaternion& old_reference_orientation );

  /** @brief Update the position of the attached_scene_node_ from the TF
   * frame specified in the Attached Frame property. */
  void updateAttachedSceneNode();

  void cameraPlacementCallback(const view_controller_msgs::CameraPlacementConstPtr &cp_ptr);
  
  /** @brief Initiate camera motion from incoming CameraTrajectory.
   *
   * @param[in] ct_ptr  incoming CameraTrajectory msg.
   */
  void cameraTrajectoryCallback(const view_controller_msgs::CameraTrajectoryConstPtr& ct_ptr);
  
  /** @brief Sets the duration the rendering has to wait for during the next update cycle.
   *
   * @params[in] pause_duration_msg  duration to wait for.
   */
  void pauseAnimationCallback(const std_msgs::Duration::ConstPtr& pause_duration_msg);

  /** @brief Transforms the camera defined by eye, focus and up into the attached frame.
   *
   * @param[in,out] eye     position of the camera.
   * @param[in,out] focus   focus point of the camera.
   * @param[in,out] up      vector pointing up from the camera.
   */
  void transformCameraToAttachedFrame(geometry_msgs::PointStamped& eye,
                                      geometry_msgs::PointStamped& focus,
                                      geometry_msgs::Vector3Stamped& up);
  
  //void setUpVectorPropertyModeDependent( const Ogre::Vector3 &vector );

  void setPropertiesFromCamera( Ogre::Camera* source_camera );

  /** @brief Begins a camera movement animation to the given goal point.
   *
   * @param[in] eye                     goal position of camera.
   * @param[in] focus                   goal focus point of camera.
   * @param[in] up                      goal vector of camera pointing up.
   * @param[in] transition_duration     duration needed for transition.
   * @param[in] interpolation_speed     the interpolation speed profile.
   */
  void beginNewTransition(const Ogre::Vector3& eye,
                          const Ogre::Vector3& focus,
                          const Ogre::Vector3& up,
                          ros::Duration transition_duration,
                          uint8_t interpolation_speed = view_controller_msgs::CameraMovement::WAVE);

  /** @brief Cancels any currently active camera movement. */
  void cancelTransition();

  /** @brief Updates the Ogre camera properties from the view controller properties. */
  void updateCamera();

  Ogre::Vector3 fixedFrameToAttachedLocal(const Ogre::Vector3 &v) { return reference_orientation_.Inverse()*(v - reference_position_); }
  Ogre::Vector3 attachedLocalToFixedFrame(const Ogre::Vector3 &v) { return reference_position_ + (reference_orientation_*v); }

  float getDistanceFromCameraToFocalPoint(); ///< Return the distance between camera and focal point.

  Ogre::Quaternion getOrientation(); ///< Return a Quaternion

protected Q_SLOTS:
  void updateTopics();


protected:    //members

  ros::NodeHandle nh_;

  rviz::BoolProperty* mouse_enabled_property_;            ///< If True, most user changes to camera state are disabled.
  rviz::EditableEnumProperty* interaction_mode_property_; ///< Select between Orbit or FPS control style.
  rviz::BoolProperty* fixed_up_property_;                 ///< If True, "up" is fixed to ... up.

  rviz::FloatProperty* distance_property_;                ///< The camera's distance from the focal point
  rviz::VectorProperty* eye_point_property_;              ///< The position of the camera.
  rviz::VectorProperty* focus_point_property_;            ///< The point around which the camera "orbits".
  rviz::VectorProperty* up_vector_property_;              ///< The up vector for the camera.
  rviz::FloatProperty* default_transition_time_property_; ///< A default time for any animation requests.

  rviz::RosTopicProperty* camera_placement_topic_property_;
  rviz::RosTopicProperty* camera_trajectory_topic_property_;

  rviz::FloatProperty* window_width_property_;            ///< The width of the rviz visualization window in pixels.
  rviz::FloatProperty* window_height_property_;           ///< The height of the rviz visualization window in pixels.

  rviz::BoolProperty* publish_view_images_property_;      ///< If True, the camera view is published as images.

  rviz::TfFrameProperty* attached_frame_property_;
  Ogre::SceneNode* attached_scene_node_;

  Ogre::Quaternion reference_orientation_;    ///< Used to store the orientation of the attached frame relative to <Fixed Frame>
  Ogre::Vector3 reference_position_;          ///< Used to store the position of the attached frame relative to <Fixed Frame>

  // Variables used during animation
  bool animate_;
  ros::WallTime transition_start_time_;
  BufferCamMovements cam_movements_buffer_;

  rviz::Shape* focal_shape_;    ///< A small ellipsoid to show the focus point.
  bool dragging_;         ///< A flag indicating the dragging state of the mouse.

  QCursor interaction_disabled_cursor_;         ///< A cursor for indicating mouse interaction is disabled.
  
  ros::Subscriber placement_subscriber_;
  ros::Subscriber trajectory_subscriber_;
  ros::Subscriber pause_animation_duration_subscriber_;

  ros::Publisher current_camera_pose_publisher_;
  ros::Publisher finished_animation_publisher_;
  image_transport::Publisher camera_view_image_publisher_;

  bool render_frame_by_frame_;
  int target_fps_;
  int rendered_frames_counter_;

  ros::WallDuration pause_animation_duration_;
};

}  // namespace rviz_animated_view_controller

#endif // RVIZ_ANIMATED_VIEW_CONTROLLER_H
