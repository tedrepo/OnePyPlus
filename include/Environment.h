#include "DataType.h"
#include "OP_DECLARE.h"

#pragma once

OP_NAMESPACE_START

class Environment {
  public:
    string instrument;
    string sys_date;
    string sys_frequency;

    string fromdate;
    string todate;
    const vector<string> tickers;
    vector<string> cur_suspended_tickers;
    map<string, vector<string>> suspended_tickers_record;

    ModuleMap<ReaderBase> readers;
    ModuleMap<BarBase> feeds;
    ModuleMap<BarBase> cleaners_feeds;
    ModuleMap<CleanerBase> cleaners;
    ModuleMap<StrategyBase> strategies;
    ModuleMap<BrokerBase> brokers;
    ModuleMap<RiskManagerBase> risk_managers;
    ModuleMap<RecorderBase> recorders;

    shared_ptr<RecorderBase> recorder; // type: op.RecorderBase

    PtrBox<SignalPtr> signals_normal_cur;                      // 动态地临时信号，会不断刷新
    PtrBox<SignalForPendingPtr> signals_pending_cur;           // 动态地临时信号，会不断刷新
    PtrBox<SignalByTriggerPtr> signals_trigger_cur;            // 动态地临时信号，会不断刷新
    PtrBox<SignalCancelTSTPtr> signals_cancel_tst_cur;         // 动态地临时信号，会不断刷新
    PtrBox<SignalCancelPendingPtr> signals_cancel_pending_cur; // 动态地临时信号，会不断刷新

    map<int, vector<PendingOrderPtr>> orders_child_of_mkt_dict; // 动态地保存跟随市价单的挂单
    PtrBox<MarketOrderPtr> orders_mkt_normal_cur;               // 动态地保存当前订单, 会不断刷新
    PtrBox<MarketOrderPtr> orders_mkt_absolute_cur;             // 动态地保存触发的挂单并成交信息，会不断刷新
    PtrBox<MarketOrderPtr> orders_mkt_submitted_cur;            // 动态地保存成交单，会不断刷新

    PtrBox<PendingOrderPtr> orders_pending; // 动态地保存挂单,触发会删除

    PtrBox<CancelTSTOrderPtr> orders_cancel_tst_cur;               // 动态地保存撤单，会不断刷新
    PtrBox<CancelPendingOrderPtr> orders_cancel_pending_cur;       // 动态地保存撤单，会不断刷新
    PtrBox<CancelTSTOrderPtr> orders_cancel_tst_submitted;         // 动态地保存撤单，会不断刷新
    PtrBox<CancelPendingOrderPtr> orders_cancel_pending_submitted; // 动态地保存撤单，会不断刷新

    //string logger = logging.getLogger("OnePy");
    shared_ptr<EventEngine> event_engine; //看看能不能设成会报错的指针
    //string cache = {};

    string execute_on_close_or_next_open;
    bool is_save_original; // 是否保存原始信号的开关
    bool is_live_trading;
    bool is_show_today_signals; // 是否显示当前最新信号的开关

    void initialize_env();

    void save_module(const string &name,
                     const shared_ptr<ReaderBase> &value);
    void save_module(const string &name,
                     const shared_ptr<CleanerBase> &value);
    void save_module(const string &name,
                     const shared_ptr<StrategyBase> &value);
    void save_module(const string &name,
                     const shared_ptr<RiskManagerBase> &value);
    void save_module(const string &name,
                     const shared_ptr<BrokerBase> &value);
    void save_module(const string &name,
                     const shared_ptr<RecorderBase> &value);

  public:
    static Environment *get_instance() noexcept {

        static Environment instance;
        return &instance;
    };
    ~Environment() = default; // dtor hidden
    Environment() noexcept;   // ctor hidden

  private:
    struct Object_Creator {
        Object_Creator() {
            Environment::get_instance();
        }
    };
    static Object_Creator _object_creator;
};

OP_NAMESPACE_END
