package cp2022.solution;

import cp2022.base.Workplace;
import cp2022.base.WorkplaceId;

import static java.lang.Thread.sleep;

public class WorkplaceWrapper extends Workplace {
    private Workplace mainWorkplace;
    private WorkshopImpl myWorkshop;
    private WorkplaceId previous;

    public WorkplaceWrapper(Workplace mainWorkplace, WorkshopImpl myWorkshop, WorkplaceId previous) {
        super(mainWorkplace.getId());
        this.mainWorkplace = mainWorkplace;
        this.myWorkshop = myWorkshop;
        this.previous = previous;
    }

    @Override
    public void use() {
        try {
            myWorkshop.getUsingWorkplace().get(previous).release();
            myWorkshop.getUsingWorkplace().get(getId()).acquire();
            mainWorkplace.use();
        } catch (InterruptedException e) {
            throw new RuntimeException("panic: unexpected thread interruption");
        }

    }
}
