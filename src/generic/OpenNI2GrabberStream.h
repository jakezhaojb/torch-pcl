#include <pcl/io/openni2_grabber.h>

// Allow single threaded access to open NI point cloud stream from main LUA thread.
// Up to maxBacklog frames are buffered.
template<typename PointT>
class OpenNI2GrabberStream
{
public:
  typedef typename pcl::PointCloud<PointT>::Ptr CloudPointPtr;
  typedef typename pcl::PointCloud<PointT>::ConstPtr CloudPointConstPtr;

  OpenNI2GrabberStream(const std::string& device_id, int maxBacklog = 30)
    : maxBacklog(maxBacklog)
    , grabber(device_id)
  {
    boost::function<void (const CloudPointConstPtr&)> cb =
         boost::bind (&OpenNI2GrabberStream::grabber_callback, this, _1);
      grabber.registerCallback(cb);
  }

  void start()
  {
    boost::unique_lock<boost::mutex> lock(queueLock);
    if (!grabber.isRunning())
      grabber.start();
  }
  
  void stop()
  {
    boost::unique_lock<boost::mutex> lock(queueLock);
    grabber.stop();
  }
  
  bool isRunning()
  {
    boost::unique_lock<boost::mutex> lock(queueLock);
    return grabber.isRunning();
  }

  void clear()
  {
    boost::unique_lock<boost::mutex> lock(queueLock);
    queue.clear();
  }
  
  size_t getCount()
  {
    boost::unique_lock<boost::mutex> lock(queueLock);
    return queue.size();
  }
  
  CloudPointPtr read(int timeout_milliseconds)
  {
    boost::unique_lock<boost::mutex> lock(queueLock);

    for (int i=0; i<2; i++)
    {
      // check if frames are available
      if (!queue.empty())
      {
        CloudPointPtr frame = queue.front(); 
        queue.pop_front();
        return frame;
      }

      // wait with timeout for frame to be captured
      if (!framesAvailable.wait_for(lock, boost::chrono::milliseconds(timeout_milliseconds)))
        break;
    }
    
    return CloudPointPtr();
  }
  
private:  
  int maxBacklog;
  std::deque<CloudPointPtr> queue;
  boost::mutex queueLock;
  boost::condition_variable framesAvailable;
  pcl::io::OpenNI2Grabber grabber;
  
  void grabber_callback(const CloudPointConstPtr &cloud)
  {
    boost::unique_lock<boost::mutex> lock(queueLock);
    if (maxBacklog >= 0 && queue.size() > maxBacklog)
      queue.pop_front();
    queue.push_back(cloud->makeShared());
    framesAvailable.notify_one();
  }
};
