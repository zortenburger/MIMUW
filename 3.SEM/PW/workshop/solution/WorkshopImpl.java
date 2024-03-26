package cp2022.solution;

import cp2022.base.Workplace;
import cp2022.base.WorkplaceId;
import cp2022.base.Workshop;

import java.util.Collection;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.Semaphore;

public class WorkshopImpl implements Workshop {

    private ConcurrentHashMap<WorkplaceId, Workplace> workplaces;
    private ConcurrentHashMap<WorkplaceId, Semaphore> waitForWorkplace;

    private ConcurrentHashMap<WorkplaceId, Semaphore> usingWorkplace;
    private ConcurrentHashMap<WorkplaceId, Semaphore> waitForEnter;
    private ConcurrentHashMap<WorkplaceId, Semaphore> waitForSwitch;
    private ConcurrentHashMap<Long, WorkplaceId> whereIsWorker;
    private ConcurrentHashMap<Long, Integer> momentOfEntrance;

    private Semaphore queueToWorkshop = new Semaphore(1, true);
    private final int numberOfWorkplaces;
    private Semaphore[] occupated;
    private int moment = 0;
    private Semaphore mutex_enter = new Semaphore(1, true);

    public WorkshopImpl(Collection<Workplace> workplaces) {
        this.workplaces = new ConcurrentHashMap<>();
        this.waitForWorkplace = new ConcurrentHashMap<>();
        this.usingWorkplace = new ConcurrentHashMap<>();
        this.waitForEnter = new ConcurrentHashMap<>();
        this.waitForSwitch = new ConcurrentHashMap<>();
        this.whereIsWorker = new ConcurrentHashMap<>();
        this.momentOfEntrance = new ConcurrentHashMap<>();
        for (Workplace workplace : workplaces) {
            WorkplaceId wid = workplace.getId();
            this.workplaces.put(wid, workplace);
            this.waitForWorkplace.put(wid, new Semaphore(1, true));
            this.usingWorkplace.put(wid, new Semaphore(1, true));
            this.waitForEnter.put(wid, new Semaphore(1, true));
            this.waitForSwitch.put(wid, new Semaphore(1, true));
        }
        numberOfWorkplaces = workplaces.size();
        occupated = new Semaphore[2 * numberOfWorkplaces];
        for (int i = 0; i < 2 * numberOfWorkplaces; i++) {
            occupated[i] = new Semaphore(1, true);
        }
        moment = 0;
    }

    @Override
    public Workplace enter(WorkplaceId wid) {
        try {
            mutex_enter.acquire();
            try {
                long myId = Thread.currentThread().getId();
                queueToWorkshop.acquire();
                occupated[moment].acquire();
                momentOfEntrance.put(myId, moment);
                moment++;
                if (moment == 2 * numberOfWorkplaces) {
                    moment = 0;
                }
                queueToWorkshop.release();

                waitForEnter.get(wid).acquire();
                waitForWorkplace.get(wid).acquire();
                usingWorkplace.get(wid).acquire();
                whereIsWorker.put(myId, wid);
                waitForEnter.get(wid).release();
                return workplaces.get(wid);
            } finally {
                mutex_enter.release();
            }
        } catch (InterruptedException e) {
            throw new RuntimeException("panic: unexpected thread interruption");
        }
    }

    @Override
    public Workplace switchTo(WorkplaceId wid) {
        try {
            long myId = Thread.currentThread().getId();
            WorkplaceId lastPlace = whereIsWorker.get(myId);
            if (wid == lastPlace) {
                return workplaces.get(wid);
            }
            waitForWorkplace.get(lastPlace).release();
            waitForSwitch.get(wid).acquire();
            waitForWorkplace.get(wid).acquire();
            whereIsWorker.put(myId, wid);
            waitForSwitch.get(wid).release();
            return new WorkplaceWrapper(workplaces.get(wid), this, lastPlace);
        } catch (InterruptedException e) {
            throw new RuntimeException("panic: unexpected thread interruption");
        }
    }

    @Override
    public void leave() {
        long myId = Thread.currentThread().getId();
        WorkplaceId lastPlace = whereIsWorker.get(myId);
        occupated[momentOfEntrance.get(myId)].release();
        momentOfEntrance.remove(myId);
        whereIsWorker.remove(myId);
        waitForWorkplace.get(lastPlace).release();
        usingWorkplace.get(lastPlace).release();
    }

    public ConcurrentHashMap<WorkplaceId, Semaphore> getUsingWorkplace() {
        return usingWorkplace;
    }
}
