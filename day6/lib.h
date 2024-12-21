#include <filesystem>
#include <map>
#include <optional>
#include <ostream>
#include <set>
#include <tuple>
#include <vector>


enum class PointType {
  Empty,
  Obstacle,
};

enum class Direction {
    Up,
    Down,
    Left,
    Right,
};

struct Point {
    int x;
    int y;

    bool operator<(const Point &other ) const;

    bool operator==(const Point &other ) const;

    bool operator!=(const Point &other ) const;
};

std::ostream& operator<<(std::ostream& os, const Point &point);

std::string toString(Direction &direction);

class Map {
    public:
        Map(uint width, uint height, std::vector<std::vector<PointType>> points);

        std::tuple<uint, uint> get_dimensions() const;

        const PointType& get_point(Point &point) const;

        std::tuple<Point, Direction> get_current_position();
        void set_current_position(Point point, Direction direction);

        std::tuple<Point, Direction> next();

        void print_local(int scope, std::optional<Point> special = std::nullopt);

        bool is_out_of_bounds();
        bool is_out_of_bounds(Point &point);

        void add_obstacle(const Point &point);
        void remove_obstacle(const Point &point);

    private:
        void print_point(Point &point);
        const uint width_;
        const uint height_;
        std::vector<std::vector<PointType>> points_;
        Point current_position_;
        Direction current_direction_;
};

std::tuple<Map, Point, Direction> read_data(const std::filesystem::path &path);

std::tuple<std::map<Point, std::set<Direction>>, bool> run_to_end(Map &map, Point start, Direction direction, bool debug = false);

std::set<Point> find_loop_points(Map &map, std::map<Point, std::set<Direction>> &visited, Point start, Direction start_direction, bool debug);
